#include "stdafx.h"
#include "BTClient.h"
#include "TcpConnetorImp.h"
#include "StringUtility.h"
#include "Utility.h"

CBTClientNet::CBTClientNet(NetType type):m_Protocal(StringUtility::FromHexString((const uint_8*)"907ec70324eb240e6490978e5346bee0805047bb",40),10,1024*16)
{
	m_isConnect = false;
	m_pConnect = NULL;
	m_Type = type;
	m_BtState = WaitHand;
	if(m_Type == TCP)
	{
		m_pConnect = new CTcpConnectImp();
	}
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
				break;
			}
			else
			{
				//连接成功
				m_isConnect = true;
				SendHandle();
				continue;
			}
		}

		if(!m_isConnect)	continue;

        //如果有数据就发送数据
        if(!WritePacket())   {break;}

        if(!ReadPacket())    {break;}
	}
}

void CBTClientNet::ProcessPacket()
{
	if(m_ReadBuffer.GetDataSize())
	{
		switch(m_BtState)
		{
		case WaitHand:
				//处理握手数据包
			HandleHandMsg();
				break;
		case WaitMsg:
		case WaitPiece:
				//处理消息数据包
			HandleCmdMsg();
			break;
		}
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
	return 0;
}

int CBTClientNet::HandleCmdMsg()
{
	uint_8 MinCmdSize = 4 + 1; //最小命令的长度 len + id
	if(m_ReadBuffer.GetDataSize() >= MinCmdSize)
	{
		int_32 len = 0;
		int_8 id;
		m_ReadBuffer.ReadInt32(len,false);
		len = ntohl(len);
		m_ReadBuffer.ReadInt8(id,false,sizeof(len));
		
		switch(id)
		{
		case BTProtocal::BITFIT:
			HandleBitfit(len);
			break;
		case BTProtocal::PIECE:
			
			break;
		}
	}
	return 0;
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

		//test
		//默认值，测试使用
		SendRequest();
        
	}
	return 0;
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
    }
    else
    {
        m_ReadBuffer.Write((void*)buf,100,ret);
    }

    ProcessPacket();
    return true;
}

bool CBTClientNet::WritePacket()
{
    if(m_WriteBuffer.GetDataSize()&& m_isConnect)
    {
        //发送数据包

        int len = (uint_32)m_pConnect->Send(m_WriteBuffer.GetData(),m_WriteBuffer.GetDataSize());
        if(len == SOCKET_ERROR)
        {
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
