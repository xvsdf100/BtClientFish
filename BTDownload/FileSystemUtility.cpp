#include "stdafx.h"
#include "FileSystemUtility.h"

CString FileSystem::GetCurrentMoudlePath()
{
	CString strPath;
	int    nPos;   

	GetModuleFileName(NULL,strPath.GetBuffer(MAX_PATH + 1),MAX_PATH);
	strPath.ReleaseBuffer();
	nPos=strPath.ReverseFind('\\');   
	strPath=strPath.Left(nPos);
	return strPath;
}
