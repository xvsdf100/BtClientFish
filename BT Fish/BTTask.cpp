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

}

bool CBTTask::Start()
{

	Init();		//如果初始化失败,这个BT任务就创建失败
	CBTClientChannel* Channel = new CBTClientChannel(this,CBTClientChannel::TCP,m_BTInfo.InfoHash,1,290);
	if(Channel->ConnectTo("127.0.0.1",7471))
	{
		Channel->Run();
	}
	else
	{
		::MessageBox(0,L"连接错误",0,0);
	}

	return true;
}

void CBTTask::Stop()
{

}

void CBTTask::Init()
{
	//创建一个必要对象
	m_DataManager = new CBTDataManager();
	m_DataManager->Init();

}
