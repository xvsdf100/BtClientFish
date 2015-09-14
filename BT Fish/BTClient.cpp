#include "stdafx.h"
#include "BTClient.h"
#include "TcpConnetorImp.h"
#include "StringUtility.h"
#include "Utility.h"

CBTClientNet::CBTClientNet(NetType type,const std::string& InfoHash,const int_32& pieceCount,const int_64& fileSize):m_Protocal(InfoHash,pieceCount,1024*16)
,m_FileSize(fileSize)
{
	m_isConnect = false;
	m_pConnect = NULL;
	m_Type = type;
	m_BtState = WaitHand;
	m_SelfChokeStatus = CHoked;
	m_PeerChokeStatus = CHoked;
    m_PieceCount = pieceCount;
	if(m_Type == TCP)
	{
		m_pConnect = new CTcpConnectImp();
	}

	InitDownFile();
}

CBTClientNet::~CBTClientNet()
{
	
}

bool CBTClientNet::ConnectTo(std::string ip,int port)
{
	bool bVal = m_pConnect->Connect(ip,port);
	if(bVal)
	{
		m_NetEvent = WSACreateEvent();
		BOOL bError = WSAEventSelect( (SOCKET)m_pConnect->GetHandle(), m_NetEvent, FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE );
		if(SOCKET_ERROR == bError)
		{
			bError = WSAGetLastError();
			TRACE("ss");
		}
	}
	return bVal;
}

void CBTClientNet::SendHandle()
{
	CommBuffer Buffer;
	m_Protocal.MakeHandle(Buffer);
    Write(&Buffer);
}

void CBTClientNet::SendBitFiled()
{

}

void CBTClientNet::SendRequest()
{
	CommBuffer Buffer;
	m_Protocal.MakeRequest(0,Buffer);
	Write(&Buffer);
}

void CBTClientNet::SendInteresed()
{
	CommBuffer Buffer;
	m_Protocal.MakeInterested(true,Buffer);
	Write(&Buffer);
}

void CBTClientNet::SendNoInteresed()
{
	CommBuffer Buffer;
	m_Protocal.MakeInterested(false,Buffer);
	Write(&Buffer);
}

void CBTClientNet::SendChoke()
{

}

void CBTClientNet::SendUnChoke()
{

}

void CBTClientNet::Run()
{
	while (true)
	{

		WSANETWORKEVENTS pEvents;
		BOOL bRet = WSAEnumNetworkEvents( (SOCKET)m_pConnect->GetHandle(), m_NetEvent, &pEvents );
		if(0!=bRet)
		{
			BOOL Error = WSAGetLastError();
			Error;
		}

		if(pEvents.lNetworkEvents & FD_CONNECT)
		{
			if ( pEvents.iErrorCode[ FD_CONNECT_BIT ] != 0 )
			{
				closesocket((SOCKET)m_pConnect->GetHandle());
                m_isConnect = false;
                AfxMessageBox(L"连接断开");
				break;
			}
			else
			{
				//连接成功
                TRACE("[CBTClientNet::Run] 连接成功 发送握手消息\n");
				m_isConnect = true;
				SendHandle();
				continue;
			}
		}

		if(!m_isConnect)	continue;

        //如果有数据就发送数据
        if(!WritePacket())   
		{
			AfxMessageBox(_T("write client disconnect"));
			break;
		}

        //如果没有读取到新的数据就不需要读取
        if(!ReadPacket())
		{
			AfxMessageBox(_T("read client disconnect"));
			break;
		}
	}
}

void CBTClientNet::ProcessPacket()
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

int CBTClientNet::HandleHandMsg()
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

int CBTClientNet::HandleCmdMsg()
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

int CBTClientNet::HandleBitfit(uint_32 len)
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

		//更新对方位图

		//默认值，测试使用
		if(m_SelfChokeStatus == CHoked)
		{
			//处理是否敢兴趣
			SendInteresed();
		}
	}

	return 1;
}


int CBTClientNet::HandleUnchoke(uint_32 len)
{
	if(len == 1)
	{
		m_ReadBuffer.Remove(0,5);
		m_SelfChokeStatus = UnChoke;

		//开始计算发送需要的
		//test
		SendRequest();
		return 1;
	}

	return -1;
}


int CBTClientNet::HandlePiece(uint_32 len)
{
	uint_32		dataLen = 0;
	uint_32		index	= 0;
	uint_32		begin	= 0;
	ByteArray	DataArray;

	if(m_Protocal.DecodePiecePacket(&m_ReadBuffer,dataLen,index,begin,DataArray))
	{
		//现在按循序请求
		WriteDataFile(0,DataArray.size(),(const byte*)&(*DataArray.begin()));
		return 1;
	}
	return -1;
}

uint_32 CBTClientNet::Write( BaseBuffer* buffer )
{
    assert(NULL != buffer);
    return m_WriteBuffer.Write((byte*)buffer->GetData(),buffer->GetDataSize(),buffer->GetDataSize());
}

bool CBTClientNet::ReadPacket()
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

bool CBTClientNet::WritePacket()
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

void CBTClientNet::Close()
{
    m_pConnect->Close();
    SAFEDEL(m_pConnect);
    m_isConnect = false;
}

void CBTClientNet::InitLocalBitmap()
{
	
}

bool CBTClientNet::InitDownFile()
{
    CFileException e;
    if(!m_File.Open(_T("D:\\testMyPiece.txt"),CFile::modeCreate | CFile::modeWrite,&e))
    {
        return false;
    }

	return true;
}

bool CBTClientNet::WriteDataFile(int_64 pos,int_32 len,const byte* data)
{
	ASSERT(m_File.m_hFile != INVALID_HANDLE_VALUE);
	ASSERT(pos < m_FileSize);
	
	//占时不填充
	//ULONGLONG pos = m_File.Seek(pos,CFile::begin);

	//if(pos > 0 )
	{
		m_File.Write(data,len);

		return true;
	}

	return false;
}

bool CBTClientNet::WriteInitData()
{
	return true;
}

void CBTClientNet::CloseFile()
{
	m_File.Close();
}

bool CBTClientNet::isDownComplete()
{
    return m_LocalBitmap.GetCount() == m_PieceCount;
}

bool CBTClientNet::GetWantDownPiece( uint_32& index,uint_32& begin,uint_32& len )
{
    //检测是否已经在请求当中
    for(uint_32 i = 0; i < m_PieceCount; i++)
    {
        if(!m_LocalBitmap.isHave(i))
        {
            //检测获取

        }
    }
    return true;
}
