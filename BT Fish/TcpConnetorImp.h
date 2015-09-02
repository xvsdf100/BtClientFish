#ifndef TCP_CONNECT_IMP
#define TCP_CONNECT_IMP
//Declare:¼òµ¥Ê±¼ätcp

#include "IConnection.h"
#include <string>

class CTcpConnectImp:public IConnect
{
public:
	CTcpConnectImp();
	~CTcpConnectImp();

	virtual bool Connect(std::string ip,int nPort);

	virtual void Close();

	virtual int GetHandle();

	virtual int Send(const byte* Data,int nSize);

	virtual int Recv(byte* Data,int nSize);

	virtual bool isValid();
private:
	void InitSocket();

private:
	int		m_Socket;
	std::string	m_SvrIp;
	int			m_SvrPort;
	bool		m_isConnect;
};




#endif

