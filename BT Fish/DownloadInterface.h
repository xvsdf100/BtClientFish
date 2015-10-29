#ifndef DOWNLOADINTERFACE_H
#define DOWNLOADINTERFACE_H
#include "DataType.h"
#include <string>

typedef void* TASK_HANDLE;

class IDownLoad
{
public:
    virtual int CreateBTTask(std::string strTorrentPath,TASK_HANDLE* pTask) = 0;
    virtual int CreateBTTask(std::string strInfoHash,int PieceCount,int PieceSize,int_64 FileSize,TASK_HANDLE* pTask) = 0;
	virtual int StartTask(TASK_HANDLE hTask) = 0;
	virtual int StopTask(TASK_HANDLE hTask) = 0;

};



//ÉêÃ÷
extern IDownLoad* GetIDownLoad();

#endif