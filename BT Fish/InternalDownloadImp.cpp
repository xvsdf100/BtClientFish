#include "InternalDownloadImp.h"

//�ڲ��ӿ�ʵ��,Ҫ����ͬ������

InternalDownloadImp::InternalDownloadImp()
{

}

InternalDownloadImp::~InternalDownloadImp()
{

}

int InternalDownloadImp::CreateBTTask( std::string strTorrentPath,TASK_HANDLE* pTask )
{
	return 0;
}

int InternalDownloadImp::CreateBTTask( std::string strInfoHash,int PieceCount,int PieceSize,int_64 FileSize,TASK_HANDLE* pTask )
{
	return 0;
}

int InternalDownloadImp::StartTask( TASK_HANDLE hTask )
{
	return 0;
}

int InternalDownloadImp::StopTask( TASK_HANDLE hTask )
{
	return 0;
}

int InternalDownloadImp::DeleteTask( TASK_HANDLE hTask )
{
	return 0;
}
