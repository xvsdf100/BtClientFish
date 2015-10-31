#include "stdafx.h"
#include "IniFile.h"

CINIFile::CINIFile(CString strPath)
{
	m_strFilePath = strPath;
}

CINIFile::~CINIFile()
{

}

CString CINIFile::GetString( CString strSection,CString strKey )
{
	CString strValue;
	GetPrivateProfileString(strSection,strKey,_T(""),strValue.GetBuffer(MAX_PATH),MAX_PATH,m_strFilePath);
	return strValue;
}

int CINIFile::GetInt( CString strSection,CString strKey )
{
	return GetPrivateProfileInt(strSection,strKey,0,m_strFilePath);
}

LONGLONG CINIFile::GetInt64( CString strSection,CString strKey )
{
	CString strValue =GetString(strSection,strKey);
	return _wtoi64(strValue.GetBuffer());
}

BOOL CINIFile::SetString( CString strSection,CString strKey,CString strValue )
{
	return WritePrivateProfileString(strSection,strKey,strValue,m_strFilePath);
}

BOOL CINIFile::SetInt( CString strSection,CString strKey,LONG iValue )
{
	CString strValue;
	strValue.Format(L"%d",iValue);
	return SetString(strSection,strKey,strValue);
}

BOOL CINIFile::SetInt64( CString strSection,CString strKey,LONGLONG iValue )
{
	CString strValue;
	strValue.Format(L"%I64d",iValue);
	return SetString(strSection,strKey,strValue);
	return 0;
}



