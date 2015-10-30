#include "InternalDownloadImp.h"
#include "BTTask.h"


//extern 实现
extern "C" IDownLoad* GetIDownLoad()
{
    return InternalDownloadImp::GetInst();
}
//内部接口实现,要处理同步处理

InternalDownloadImp::InternalDownloadImp()
{
    m_bStop = false;
}

InternalDownloadImp::~InternalDownloadImp()
{

}


int InternalDownloadImp::Init(std::string ip,uint_16 port)
{
    m_hTread = (HANDLE)_beginthread(InternalDownloadImp::DownLoadLoop,0,this);

    if(INVALID_HANDLE_VALUE == m_hTread)    
    {
        return 1;
    }

    m_strIP = ip;
    m_u16Port = port;
    return 0;
}


int InternalDownloadImp::CreateBTTask( std::string strTorrentPath,std::string strPath,TASK_HANDLE* hTask )
{
    if(strTorrentPath.empty())  return 1;
    m_SectionLock.Lock();
    m_SectionLock.Unlock();
	return 0;
}

int InternalDownloadImp::CreateBTTask( std::string strInfoHash,int PieceCount,int PieceSize,int_64 FileSize,std::string strPath,TASK_HANDLE* hTask )
{
    if(strInfoHash.empty() || 0 == PieceCount || 0 == PieceSize || 0 == FileSize || NULL == hTask) return 1;

    CBTTask::TaskInfo info;
    info.SavePath = strPath;
    info.Info.FileSize = FileSize;
    info.Info.InfoHash = strInfoHash;
    info.Info.PieceCount = PieceCount;
    info.Info.PieceSize = PieceSize;
    info.Info.TorrentPath = "";
    CBTTask* pTask = new CBTTask(info);

    AddTask((TASK_HANDLE)pTask,pTask);
    *hTask = (TASK_HANDLE)pTask;
	return 0;
}

int InternalDownloadImp::StartTask( TASK_HANDLE hTask )
{
    if(isValidTaskHandle(hTask))
    {
        CDownloadTask* pTask = (CDownloadTask*)hTask;
        return pTask->Start();
    }
	return 1;
}

int InternalDownloadImp::StopTask( TASK_HANDLE hTask )
{
    if(isValidTaskHandle(hTask))
    {
        CDownloadTask* pTask = (CDownloadTask*)hTask;
        return pTask->Stop();
    }
	return 0;
}

int InternalDownloadImp::DeleteTask( TASK_HANDLE hTask )
{
	return 0;
}

void InternalDownloadImp::DownLoadLoop( void* lpParam )
{
    InternalDownloadImp::GetInst()->Run();
}

void InternalDownloadImp::Run()
{
    do 
    {
        m_SectionLock.Lock();
        CDownloadTask* pDownloadTask = NULL;
        for (TaskMap::iterator it = m_TaskMap.begin(); it != m_TaskMap.end(); it++)
        {
            pDownloadTask = it->second;
            if(pDownloadTask->GetDownloadStatus() == TS_START)
            {
                pDownloadTask->Run();
            }

        }
        m_SectionLock.Unlock();
        Sleep(10);
        //等待任务
    } while (!m_bStop);
}

bool InternalDownloadImp::AddTask( TASK_HANDLE hTask,CDownloadTask* pTask )
{
    assert(NULL != pTask);
    assert(NULL != hTask);
    std::pair<TaskMap::iterator,bool> bRet;

    m_SectionLock.Lock();
    bRet = m_TaskMap.insert(std::make_pair(hTask,pTask));
    m_SectionLock.Unlock();
    if(bRet.second)
        return true;
    else
        return false;
}

bool InternalDownloadImp::isValidTaskHandle( TASK_HANDLE hTask )
{
    if(m_TaskMap.find(hTask) != m_TaskMap.end())
    {
        return true;
    }

    return false;
}


