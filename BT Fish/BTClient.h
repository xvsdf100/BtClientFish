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
#include "BTBitmap.h"

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

	enum ChokeStatus
	{
		UnChoke,
		CHoked,
	};

public:
	CBTClientNet(NetType type,const std::string& InfoHash,const int_32& pieceCount,const int_64& fileSize);
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

	int HandleUnchoke(uint_32 len);

	//����Piece
	int HandlePiece(uint_32 len);

private:
    uint_32 Write(BaseBuffer* buffer);

    void Close();

	void InitLocalBitmap();

private:
	//�ļ����в���
	bool InitDownFile();

	bool WriteDataFile(int_64 pos,int_32 len,const byte* data);

	//ȫ��д0д��ʼ������
	bool WriteInitData();

	void CloseFile();

private:
	bool		m_isConnect;
	IConnect*	m_pConnect;
	NetType		m_Type;
	LockBuffer	m_WriteBuffer;
	LockBuffer	m_ReadBuffer;
	BTProtocal	m_Protocal;
	BTNetState	m_BtState;
	WSAEVENT	m_NetEvent;
	CBTBitmap	m_RemoteBitmap;
	CBTBitmap	m_LocalBitmap;
	ChokeStatus	m_SelfChokeStatus;
	ChokeStatus	m_PeerChokeStatus;
	uint_64		m_FileSize;

private:
	CFile		m_File;

private:
	CBTClientNet(CBTClientNet&);

};
#endif