#ifndef BTCLIENT_H
#define BTCLIENT_H
/************************************************************************/
/* ˵������װBTClien����                                                */
/* ���ߣ����															*/
/* ��ϵ��littlefish@qq.com												*/
/************************************************************************/

#include "IConnection.h"
#include <string>
#include "BaseThread.h"
#include "LockBuffer.h"
#include "BTProtocal.h"

class CBTClientNet:public BaseThread
{
public:
	enum NetType
	{
		TCP,
		UDP
	};

	enum BTNetState
	{
		NetInit,
		WaitHand,		//�ȴ��Է�������Ϣ	
		WaitMsg,		//�ȴ���Ϣ
		WaitPiece,		//�ȴ�piece
	};

public:
	CBTClientNet(NetType type);
	~CBTClientNet();

	bool ConnectTo(std::string ip,int port);

	virtual void Run();
#ifndef _DEBUG 
private:
#endif
	//��������Э��
	void SendHandle();

	//����λͼ
	void SendBitFiled();

	//����Request
	void SendRequest();

	//���͸���Ȥ
	void SendInteresed();

	//���Ͳ�����Ȥ
	void SendNoInteresed();

	//����choke
	void SendChoke();

	//����Unchoke
	void SendUnChoke();

private:
    //��ȡ����
    bool ReadPacket();

    //д����
    bool WritePacket();

	//��ȡ���յ�Packet
	void ProcessPacket();

	//����������Ϣ
	// 0 ���� ��û�н�����ɣ� 1���������ɣ� -1������������
	int HandleHandMsg();

	//������ͨ��������Ϣ
	int HandleCmdMsg();

	int HandleBitfit(uint_32 len);

private:
    uint_32 Write(BaseBuffer* buffer);

    void Close();

private:
	bool		m_isConnect;
	IConnect*	m_pConnect;
	NetType		m_Type;
	LockBuffer	m_WriteBuffer;
	LockBuffer	m_ReadBuffer;
	BTProtocal	m_Protocal;
	BTNetState	m_BtState;
	WSAEVENT	m_NetEvent;

private:
	CBTClientNet(CBTClientNet&);

};
#endif