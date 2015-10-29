#ifndef DOWNLOADINTERFACE_H
#define DOWNLOADINTERFACE_H
#include "DataType.h"
#include <string>

class ITask
{
public:
    virtual Start();
    virtual Stop();
};

class IDownLoad
{
public:
    virtual int CreateBTTask(std::string strTorrentPath) = 0;
    virtual int CreateBTTask(std::string strInfoHash,int PieceCount,int PieceSize,int_64 FileSize) = 0;

};
#endif