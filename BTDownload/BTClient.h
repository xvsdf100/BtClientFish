#ifndef BTCLIENT_H
#define BTCLIENT_H
/************************************************************************/
/* ˵������װBTClien����                                                */
/* ���ߣ����															*/
/* ��ϵ��littlefishcool@qq.com											*/
/************************************************************************/

//BTClient Э��Ľ���

//tip:��������������ӣ������Ϳ���������������,Ŀǰֻ֧��ͬʱֻ�ܴ�һ��peer����

#include "IConnection.h"
#include <WinSock2.h>
#include <string>
#include "BaseThread.h"
#include "LockBuffer.h"
#include "BTProtocal.h"
#include "BTBitmap.h"


class CBTTask;
class CBTClientChannel
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

    typedef struct tagRequestDataBlock
    {
        uint_32 index;
        uint_32 begin;
        uint_32 len;
    }RequestDataBlock;

    typedef std::vector<RequestDataBlock>    RequestDataBlockArray;

public:
	CBTClientChannel(CBTTask* task, NetType type,const std::string& InfoHash,const int_32& pieceCount,const int_64& fileSize);
	~CBTClientChannel();

	bool ConnectTo(std::string ip,int port);

	virtual int Run();
#ifndef _DEBUG 
private:
#endif
	//��������Э��
	void SendHandle();

	//����λͼ
	void SendBitFiled();

	//����Request
	void SendRequest(uint_32 index,uint_32 begin,uint_32 len);

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

private:
	CBTTask*				m_pTask;
	bool		            m_isConnect;
	IConnect*	            m_pConnect;
	NetType		            m_Type;
	LockBuffer	            m_WriteBuffer;
	LockBuffer	            m_ReadBuffer;
	BTProtocal	            m_Protocal;
	BTNetState	            m_BtState;
	WSAEVENT	            m_NetEvent;
	CBTBitmap	            m_RemoteBitmap;
	ChokeStatus	            m_SelfChokeStatus;
	ChokeStatus	            m_PeerChokeStatus;
	DataRange				m_NeedRange;
	uint_32					m_DownLoadByte;
	CommBuffer				m_PieceBuffer;


private:
	CBTClientChannel(CBTClientChannel&);

};
#endif