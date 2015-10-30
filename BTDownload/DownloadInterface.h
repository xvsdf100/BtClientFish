#ifndef DOWNLOADINTERFACE_H
#define DOWNLOADINTERFACE_H
#include "DataType.h"
#include <string>


typedef void* TASK_HANDLE;

enum TASK_DOWN_STATUS
{
    TS_NOITEM,
    TS_CREATE,
    TS_START,
    TS_STOP,
    TS_COMPELETE,
    TS_ERROR,
};

class IDownLoad
{
public:
    virtual int Init(std::string ip, uint_16 port) = 0;
    virtual int CreateBTTask(std::string strTorrentPath,std::string strPath,TASK_HANDLE* pTask) = 0;
    virtual int CreateBTTask(std::string strInfoHash,int PieceCount,int PieceSize,int_64 FileSize,std::string strPath,TASK_HANDLE* pTask) = 0;
	virtual int StartTask(TASK_HANDLE hTask) = 0;
	virtual int StopTask(TASK_HANDLE hTask) = 0;

};



//ÉêÃ÷
extern "C" IDownLoad* GetIDownLoad();

#endif