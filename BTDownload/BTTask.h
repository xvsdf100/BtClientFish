#ifndef BTTASK_H
#define BTTASK_H
#include <string>
#include <vector>
#include "Utility.h"
#include "BTClient.h"
#include "BTDataManager.h"
#include "DownloadTask.h"
#include "BTDefine.h"
using namespace BT;

class CBTTask:public CDownloadTask
{

private:
	friend class CBTClientChannel;
public:
	typedef std::vector<CBTClientChannel*> BTClientChannelArray;

	typedef struct tagBTInfo
	{
        TorrentInfo Info;
        std::string SavePath;
	}TaskInfo;

public:
	CBTTask(const TaskInfo& Info);
	~CBTTask();

    virtual int Start();
    virtual int Stop();
    virtual int Run();

private:
	void Init();

private:
	TaskInfo					m_BTInfo;
	BTClientChannelArray	m_ChannelArray;	
	CBTDataManager*			m_DataManager;
    CBTClientChannel*       m_pMainChannel;

};
#endif