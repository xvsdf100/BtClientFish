#ifndef BTCLIENT_H
#define BTCLIENT_H
/************************************************************************/
/* 说明：封装BTClien对象                                                */
/* 作者：鱼儿															*/
/* 联系：littlefish@qq.com												*/
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
		WaitHand,		//等待对方握手消息	
		WaitMsg,		//等待消息
		WaitPiece,		//等待piece
	};

public:
	CBTClientNet(NetType type);
	~CBTClientNet();

	bool ConnectTo(std::string ip,int port);

	virtual void Run();
#ifndef _DEBUG 
private:
#endif
	//发送握手协议
	void SendHandle();

	//发送位图
	void SendBitFiled();

	//发送Request
	void SendRequest();

	//发送感兴趣
	void SendInteresed();

	//发送不刚兴趣
	void SendNoInteresed();

	//发送choke
	void SendChoke();

	//发送Unchoke
	void SendUnChoke();

private:
	//读取接收到Packet
	void ProcessPacket();

	//处理握手消息
	// 0 代表 包没有解析完成， 1代表解析完成， -1代表错误解析包
	int HandleHandMsg();

	//处理普通的命令消息
	int HandleCmdMsg();

	int HandleBitfit(uint_32 len);

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