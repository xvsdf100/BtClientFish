#include "BTTask.h"


CBTTask::CBTTask(const BTInfo& Info)
{
	m_BTInfo = Info;
	m_Status = 0;
	m_DataManager = NULL;
}

CBTTask::~CBTTask()
{

}

bool CBTTask::Start()
{
	CBTClientChannel* Channel = new CBTClientChannel(CBTClientChannel::TCP,m_BTInfo.InfoHash,1,290);
	if(Channel->ConnectTo("127.0.0.1",6666))
	{
		TRACE0(L"���ӳɹ�,��ʼ����run");
		Channel->Run();
	}
	else
	{
		AfxMessageBox(L"���Ӵ���");
	}

	return true;
}

void CBTTask::Stop()
{

}

void CBTTask::Init()
{
	//����һ����Ҫ����
	m_DataManager = new CBTDataManager();
	

}
