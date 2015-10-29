#include "Utility.h"
#include "BTTask.h"

CBTTask::CBTTask()
{
	//test
}

CBTTask::CBTTask(const BTInfo& Info)
{
	m_BTInfo = Info;
	m_Status = 0;
	m_DataManager = NULL;
}


CBTTask::~CBTTask()
{
    SAFEDEL(m_DataManager);
}

bool CBTTask::Start()
{

	Init();		//�����ʼ��ʧ��,���BT����ʹ���ʧ��
	CBTClientChannel* Channel = new CBTClientChannel(this,CBTClientChannel::TCP,m_BTInfo.InfoHash,1,290);
	if(Channel->ConnectTo("127.0.0.1",11038))
	{
		Channel->Run();
	}
	else
	{
		::MessageBox(0,L"���Ӵ���",0,0);
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
	m_DataManager->Init();

}
