#include "Utility.h"
#include "BTTask.h"
#include "InternalDownloadImp.h"

CBTTask::CBTTask(const TaskInfo& Info)
{
	m_BTInfo = Info;
	m_DataManager = NULL;
    m_pMainChannel = NULL;
    Init();
}


CBTTask::~CBTTask()
{
    SAFEDEL(m_DataManager);
    SAFEDEL(m_pMainChannel);
}

int CBTTask::Start()
{

    if(m_emTaskStatus == TS_NOITEM || m_emTaskStatus == TS_STOP || m_emTaskStatus == TS_ERROR)
    {
        CBTClientChannel* Channel = new CBTClientChannel(this,CBTClientChannel::TCP,m_BTInfo.Info.InfoHash,1,290);
        std::string strIp = InternalDownloadImp::GetInst()->GetIP();
        uint_16 u16Port = InternalDownloadImp::GetInst()->GetPort();
        if(!Channel->ConnectTo(strIp,u16Port))
        {
            delete Channel;
            return 1;
        }

        m_pMainChannel = Channel;
    }
    else
    {
        return 1;
    }
	
    m_emTaskStatus = TS_START;
	return 0;
}

int CBTTask::Stop()
{
    m_emTaskStatus = TS_STOP;
    return 0;
}

void CBTTask::Init()
{
	//创建一个必要对象

    BT::TorrentInfo Info;
    Info.FileSize = m_BTInfo.Info.FileSize;
    Info.InfoHash = m_BTInfo.Info.InfoHash;
    Info.PieceCount = m_BTInfo.Info.PieceCount;
    Info.PieceSize = m_BTInfo.Info.PieceSize;

	m_DataManager = new CBTDataManager(Info,m_BTInfo.SavePath);
	m_DataManager->Init();

}

int CBTTask::Run()
{
    if(NULL != m_pMainChannel)
    {
        m_pMainChannel->Run();
        return 0;
    }

    return 1;
}
