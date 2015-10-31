#include "stdafx.h"
#include "BTClient.h"
#include "TcpConnetorImp.h"
#include "StringUtility.h"
#include "Utility.h"
#include "BTTask.h"

CBTClientChannel::CBTClientChannel(CBTTask* task, NetType type,const std::string& InfoHash,const int_32& pieceCount,const int_64& fileSize):m_Protocal(InfoHash,pieceCount,1024*16)
{
	m_isConnect = false;
	m_pConnect = NULL;
	m_Type = type;
	m_BtState = WaitHand;
	m_SelfChokeStatus = CHoked;
	m_PeerChokeStatus = CHoked;
	if(m_Type == TCP)
	{
		m_pConnect = new CTcpConnectImp();
	}

	assert(NULL != task);
	m_pTask = task;
	m_DownLoadByte = 0;
}

CBTClientChannel::~CBTClientChannel()
{
	
}

bool CBTClientChannel::ConnectTo(std::string ip,int port)
{
	bool bVal = m_pConnect->Connect(ip,port);
	if(bVal)
	{
		m_NetEvent = WSACreateEvent();
		BOOL bError = WSAEventSelect( (SOCKET)m_pConnect->GetHandle(), m_NetEvent, FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE );
		if(SOCKET_ERROR == bError)
		{
			bError = WSAGetLastError();
            char MsgError[100];
            sprintf(MsgError,"[CBTClientChannel::ConnectTo] Error:%d\n",bError);
            TRACE(MsgError);
		}
	}
	return bVal;
}

void CBTClientChannel::SendHandle()
{
	CommBuffer Buffer;
	m_Protocal.MakeHandle(Buffer);
    Write(&Buffer);
}

void CBTClientChannel::SendBitFiled()
{

}

void CBTClientChannel::SendRequest(uint_32 index,uint_32 begin,uint_32 len)
{
	CommBuffer Buffer;
	//暂时这样赋值,给最指定的值
	m_Protocal.MakeRequest(index,begin,len,Buffer);
	Write(&Buffer);

	CString strMsg;
	strMsg.Format(L"[CBTClientChannel::SendRequest]:len:%d,index:%d,begin:%d\n",len,index,begin);
	TRACE(strMsg);
}

void CBTClientChannel::SendInteresed()
{
	CommBuffer Buffer;
	m_Protocal.MakeInterested(true,Buffer);
	Write(&Buffer);
}

void CBTClientChannel::SendNoInteresed()
{
	CommBuffer Buffer;
	m_Protocal.MakeInterested(false,Buffer);
	Write(&Buffer);
}

void CBTClientChannel::SendChoke()
{

}

void CBTClientChannel::SendUnChoke()
{

}

int CBTClientChannel::Run()
{

    WSANETWORKEVENTS pEvents;
    BOOL bRet = WSAEnumNetworkEvents( (SOCKET)m_pConnect->GetHandle(), m_NetEvent, &pEvents );
    if(0!=bRet)
    {
        BOOL Error = WSAGetLastError();
        char MsgError[100];
        sprintf(MsgError,"[CBTClientChannel::Run] WSANetworkEvent Error:%d\n",Error);
        TRACE(MsgError);
        return 1;
    }

    if(pEvents.lNetworkEvents & FD_CONNECT)
    {
        if ( pEvents.iErrorCode[ FD_CONNECT_BIT ] != 0 )
        {
            closesocket((SOCKET)m_pConnect->GetHandle());
            m_isConnect = false;
            AfxMessageBox(L"连接断开");
            return 1;
        }
        else
        {
            //连接成功
            TRACE("[CBTClientNet::Run] 连接成功 发送握手消息\n");
            m_isConnect = true;
            SendHandle();
        }
    }

    if(!m_isConnect)	
    {
        //没有连接
        return 0;
    }

    //如果有数据就发送数据
    if(!WritePacket())   
    {
        AfxMessageBox(_T("write client disconnect"));
        return 1;
    }

    //如果没有读取到新的数据就不需要读取
    if(!ReadPacket())
    {
        AfxMessageBox(_T("read client disconnect"));
       return 1;
    }

    return 0;

}

void CBTClientChannel::ProcessPacket()
{

    while (m_ReadBuffer.GetDataSize())
    {
        int ret = 0;
        switch(m_BtState)
        {
        case WaitHand:
            //处理握手数据包
            ret = HandleHandMsg();
            break;
        case WaitMsg:
        case WaitPiece:
            //处理消息数据包
            ret = HandleCmdMsg();
            break;
        }

        if(1 != ret)    break;
    }
}

int CBTClientChannel::HandleHandMsg()
{
	if(m_ReadBuffer.GetDataSize() >= 68)
	{
		int_8 Len = 0;
		m_ReadBuffer.ReadInt8(Len);
		if(19 != Len)	return -1;
		std::string BtProtocal;
		m_ReadBuffer.ReadString(BtProtocal,Len);
		if(BtProtocal != "BitTorrent protocol")	return -1;
		char Reserved[8];
		m_ReadBuffer.Read(Reserved,8,8);
		std::string RequestInfoHash;
		m_ReadBuffer.ReadString(RequestInfoHash,20);
		std::string PeerID;
		m_ReadBuffer.ReadString(PeerID,20);
		
		m_BtState = WaitMsg;
	}

	return 1;
}

int CBTClientChannel::HandleCmdMsg()
{
	int ret = 0;
	uint_8 MinCmdSize = 4 + 1; //最小命令的长度 len + id
    uint_32 DataSize = m_ReadBuffer.GetDataSize();
	if(DataSize >= MinCmdSize)
	{
		int_32 len = 0;
		int_8 id;
		m_ReadBuffer.ReadInt32(len,false);
		len = ntohl(len);
		m_ReadBuffer.ReadInt8(id,false,sizeof(len));
        TRACE("[CBTClientNet::HandleCmdMsg] MSG ID:%d ReadBuffer:%d CmdLen:%d\n",id,DataSize,len);

		//检测是否为一个完整的数据包
		if(DataSize >= (len + sizeof(len)))
		{
			switch(id)
			{
			case BTProtocal::BITFIT:
				ret = HandleBitfit(len);
				break;
			case BTProtocal::PIECE:
				ret = HandlePiece(len);
				break;
			case BTProtocal::UNCHOKE:
				ret = HandleUnchoke(len);
				break;
            default:
                AfxMessageBox(_T("协议未知报错"));
                break;
			}
		}
	}

	return ret;
}

int CBTClientChannel::HandleBitfit(uint_32 len)
{
	if(m_ReadBuffer.GetDataSize() >= len)
	{
		int_32 nLen;
		int_8 id;
		int_32 nBitfitLen = 0;
		int_8* pBitfit = NULL;
		m_ReadBuffer.ReadInt32(nLen);
		nLen = ntohl(nLen);
		m_ReadBuffer.ReadInt8(id);
		nBitfitLen = nLen - sizeof(id);
		pBitfit = new int_8[nBitfitLen];
		m_ReadBuffer.Read(pBitfit,nBitfitLen,nBitfitLen);
		delete pBitfit;

		m_BtState = WaitPiece;

		TRACE("[CBTClientChannel::HandleBitfit]");

		//更新对方位图

		//获取自己需要下载的位图，以后加上通过对方的位图来获取自己可以从对方下载哪些位图，然后再下载。
		DataRange range;
		//这里暂时认为对方都有，遍历自己没有就可以下载。
		assert(m_pTask->m_DataManager->GetNoneDownDataRange(range));
		m_NeedRange = range;

		//默认值，测试使用
		if(m_SelfChokeStatus == CHoked)
		{
			//处理是否敢兴趣
			SendInteresed();
		}
	}

	return 1;
}


int CBTClientChannel::HandleUnchoke(uint_32 len)
{
	TRACE("[CBTClientChannel::HandleUnchoke]");
	if(len == 1)
	{
		m_ReadBuffer.Remove(0,5);
		m_SelfChokeStatus = UnChoke;
		uint_32 ulen = m_NeedRange.len <= 16*1024 ? m_NeedRange.len : 16*1024;
		SendRequest(m_NeedRange.index,m_DownLoadByte,ulen);
		return 1;
	}

	return -1;
}


int CBTClientChannel::HandlePiece(uint_32 len)
{
	uint_32		dataLen = 0;
	uint_32		index	= 0;
	uint_32		begin	= 0;
	ByteArray	DataArray;

	if(m_Protocal.DecodePiecePacket(&m_ReadBuffer,dataLen,index,begin,DataArray))
	{
		CString strMsg;
		strMsg.Format(L"[CBTClientChannel::HandlePiece]:len:%d,index:%d,begin:%d\n",dataLen,index,begin);
		TRACE(strMsg);
		m_DownLoadByte += dataLen;
		m_NeedRange.len -= dataLen;
        m_PieceBuffer.Write(&DataArray[0],DataArray.size(),DataArray.size());
		if(0 == m_NeedRange.len)
		{	
			m_NeedRange.len = m_DownLoadByte;
			if(!m_pTask->m_DataManager->AddDownedDataRange(m_NeedRange,(uint_8*)m_PieceBuffer.GetData()))
			{
				OutputDebugString(L"On Piece Ok");

				//先检测是否下载完成
				if(m_pTask->m_DataManager->isComplete())
				{
					//触发关闭任务
					TRACE("[CBTClientChannel::HandlePiece] 已经下载完成");
					return 1;
				}
				//获取自己需要下载的位图，以后加上通过对方的位图来获取自己可以从对方下载哪些位图，然后再下载。
				DataRange range;
				//这里暂时认为对方都有，遍历自己没有就可以下载。
				if(m_pTask->m_DataManager->GetNoneDownDataRange(range))
				{
					m_NeedRange = range;
					uint_32 ulen = m_NeedRange.len <= 16*1024 ? m_NeedRange.len : 16*1024;
					m_DownLoadByte = 0;
					SendRequest(m_NeedRange.index,m_DownLoadByte,ulen);
				}

			}
            m_PieceBuffer.Clear();
            return 1;
		}
		else
		{
			uint_32 ulen = m_NeedRange.len <= 16*1024 ? m_NeedRange.len : 16*1024;
			SendRequest(m_NeedRange.index,m_DownLoadByte,ulen);
		}

        

	}
	return -1;
}

uint_32 CBTClientChannel::Write( BaseBuffer* buffer )
{
    assert(NULL != buffer);
    return m_WriteBuffer.Write((byte*)buffer->GetData(),buffer->GetDataSize(),buffer->GetDataSize());
}

bool CBTClientChannel::ReadPacket()
{
    const uint_32 bufLen = 1024*8;
    byte buf[bufLen];

    while(TRUE)
    {
        int ret = m_pConnect->Recv((byte*)buf,bufLen);
        if(ret <= 0)
        {
            ret = WSAGetLastError();
            if(ret != WSAEWOULDBLOCK)
            {
                //发生错误跳出
                AfxMessageBox(_T("Client error"));
                this->Close();
                return false;
            }

            break;
        }
        else
        {
            m_ReadBuffer.Write((void*)buf,bufLen,ret);
        }
    }

    ProcessPacket();
    return true;
}

bool CBTClientChannel::WritePacket()
{
    if(m_WriteBuffer.GetDataSize() > 0&& m_isConnect)
    {
        //发送数据包
		int buffsize = m_WriteBuffer.GetDataSize();
		TRACE("[CBTClientNet::WritePacket] buffsize:%d\n",buffsize);
        int len = (uint_32)m_pConnect->Send(m_WriteBuffer.GetData(),m_WriteBuffer.GetDataSize());
        if(len == SOCKET_ERROR)
        {
			BOOL lastError = WSAGetLastError();
			TRACE("WritePacket lastError:%d",lastError);
            if(WSAEWOULDBLOCK != WSAGetLastError())
            {
                this->Close();
                return false;
            }
        }
        else
        {
            m_WriteBuffer.Remove(0,len);
        }
    }
    return true;
}

void CBTClientChannel::Close()
{
    m_pConnect->Close();
    SAFEDEL(m_pConnect);
    m_isConnect = false;
}

void CBTClientChannel::InitLocalBitmap()
{
	
}

bool CBTClientChannel::InitDownFile()
{
	return true;
}

bool CBTClientChannel::WriteDataFile(int_64 pos,int_32 len,const byte* data)
{


	return true;
}

bool CBTClientChannel::WriteInitData()
{
	return true;
}

void CBTClientChannel::CloseFile()
{

}

