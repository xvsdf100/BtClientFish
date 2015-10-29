#ifndef INTERNALDOWNLOADIMP_H
#define INTERNALDOWNLOADIMP_H
#include "DownloadInterface.h"
#include <map>

class InternalDownloadImp:public IDownLoad
{
	typedef std::map<TASK_HANDLE,void*> TaskMap;
public:
	InternalDownloadImp();
	~InternalDownloadImp();

	 virtual int CreateBTTask(std::string strTorrentPath,TASK_HANDLE* pTask);
	 virtual int CreateBTTask(std::string strInfoHash,int PieceCount,int PieceSize,int_64 FileSize,TASK_HANDLE* pTask);
	 virtual int StartTask(TASK_HANDLE hTask);
	 virtual int StopTask(TASK_HANDLE hTask);
	 virtual int DeleteTask(TASK_HANDLE hTask);


private:
	TaskMap		m_TaskMap;

};
#endif