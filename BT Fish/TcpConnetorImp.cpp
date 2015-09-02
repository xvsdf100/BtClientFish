#include "stdafx.h"
#include "TcpConnetorImp.h"


CTcpConnectImp::CTcpConnectImp()
{
	m_Socket = INVALID_SOCKET;
	m_isConnect = false;
	InitSocket();
}

CTcpConnectImp::~CTcpConnectImp()
{

}

bool CTcpConnectImp::Connect(std::string ip,int nPort)
{
	assert(!m_isConnect);
	m_SvrIp = ip;
	m_SvrPort = nPort;

	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr(m_SvrIp.c_str());
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_SvrPort);

	int ret = ::WSAConnect(m_Socket,(SOCKADDR*)&addr,sizeof(addr),NULL,NULL,NULL,NULL);
	if(SOCKET_ERROR == ret)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)	
		{
			return false;
		}
		else
		{
			m_isConnect = true;
		}
	}

	return true;
}

int CTcpConnectImp::Send(const byte* Data,int nSize)
{
	int ret = ::send(m_Socket,(const char*)Data,nSize,0);
	return ret;
}

int CTcpConnectImp::Recv(byte* Data,int nSize)
{
	int ret = ::recv(m_Socket,(char*)Data,nSize,0);
	return ret;
}

void CTcpConnectImp::Close()
{
	::closesocket(m_Socket);
}

int CTcpConnectImp::GetHandle()
{
	return (int)m_Socket;
}

void CTcpConnectImp::InitSocket()
{
	ASSERT(m_Socket == INVALID_SOCKET);
	m_Socket = socket(AF_INET,SOCK_STREAM,0);

	DWORD dwValue = 1;
	ioctlsocket(	// Call Windows Sockets ioctlsocket to control the input/output mode of our new socket
		m_Socket,	// Give it our new socket
		FIONBIO,	// Select the option for blocking i/o, should the program wait on read and write calls, or keep going?
		&dwValue ); // Nonzero, it should keep going
}

bool CTcpConnectImp::isValid()
{
	return m_Socket == INVALID_SOCKET ? false : true;
}

