#ifndef INIFILE_H
#define INIFILE_H


class CINIFile
{
public:
	CINIFile(CString strPath);
	~CINIFile();


	CString		GetString(CString strSection,CString strKey);
	int			GetInt(CString strSection,CString strKey);
	LONGLONG	GetInt64(CString strSection,CString strKey);

	//…Ë÷√
	BOOL		SetString(CString strSection,CString strKey,CString strValue);
	BOOL		SetInt(CString strSection,CString strKey,LONG iValue);
	BOOL		SetInt64(CString strSection,CString strKey,LONGLONG iValue);

private:
	CString m_strFilePath;
};
#endif