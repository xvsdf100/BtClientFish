#ifndef INTERNALDOWNLOADIMP_H
#define INTERNALDOWNLOADIMP_H
#include <map>
#include "Utility.h"
#include "DownloadTask.h"
#include "DownloadInterface.h"


class InternalDownloadImp:public IDownLoad
{
	typedef std::map<TASK_HANDLE,CDownloadTask*> TaskMap;
    typedef std::pair<TASK_HANDLE,CDownloadTask*>   TaskMapPair;
public:
	InternalDownloadImp();
	~InternalDownloadImp();

     virtual int Init(std::string ip,uint_16 port);
	 virtual int CreateBTTask(std::string strTorrentPath,std::string strPath,TASK_HANDLE* hTask);
	 virtual int CreateBTTask(std::string strInfoHash,int PieceCount,int PieceSize,int_64 FileSize,std::string strPath,TASK_HANDLE* hTask);
	 virtual int StartTask(TASK_HANDLE hTask);
	 virtual int StopTask(TASK_HANDLE hTask);
	 virtual int DeleteTask(TASK_HANDLE hTask);


public:
    std::string GetIP() {return m_strIP;}

    uint_16 GetPort() {return m_u16Port;}

public:
    static InternalDownloadImp* GetInst()
    {
        static InternalDownloadImp* g_Imp = NULL;
        if(NULL == g_Imp)   g_Imp = new InternalDownloadImp();
        return g_Imp;
    }


protected:
    //主线程处理代码
    static void DownLoadLoop(void* lpParam);

    virtual void Run();

private:
    bool AddTask(TASK_HANDLE hTask,CDownloadTask* pTask);
    bool isValidTaskHandle(TASK_HANDLE hTask);
private:

	CCriticalSection    m_SectionLock;

	TaskMap		        m_TaskMap;
    bool                m_bStop;
    HANDLE              m_hTread;
    std::string         m_strIP;
    uint_16             m_u16Port;

};
#endif