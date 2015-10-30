#include "DownloadTask.h"

CDownloadTask::CDownloadTask()
{
	m_emTaskStatus = TS_NOITEM;
}

CDownloadTask::~CDownloadTask()
{

}

TASK_DOWN_STATUS CDownloadTask::GetDownloadStatus()
{
	return m_emTaskStatus;
}

