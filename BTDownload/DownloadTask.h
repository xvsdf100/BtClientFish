#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H
#include "DownloadInterface.h"


class CDownloadTask
{
public:
	CDownloadTask();
	virtual ~CDownloadTask();

	virtual int Start() = 0;
	virtual int Stop() = 0;
	virtual int Run() = 0;

	TASK_DOWN_STATUS GetDownloadStatus();

protected:
	TASK_DOWN_STATUS m_emTaskStatus;
};
#endif