#include "StdAfx.h"

#include "SeedResolver.h"

using namespace std;

#pragma warning(disable : 4244 4996)


UINT nDebug1 = 0;
UINT nDebug2 = 0;

CSeedResolver::CSeedResolver(const CString& SeedFileName)
{
	ASSERT(!SeedFileName.IsEmpty());

	SeedInfo.Seed_FileName = SeedFileName;
}

CSeedResolver::~CSeedResolver(void)
{
	//����
	DeallocAll();
}

BOOL CSeedResolver::Resolve()
{
	char* pBuffer = NULL;
	char* pShadowBuffer = NULL;

	HANDLE hFile = CreateFile(SeedInfo.Seed_FileName, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	CFile file(hFile);
	m_SeedFileSize =(UINT) file.GetLength();
	m_Position = 0;

	if(!m_SeedFileSize) return FALSE;

	pBuffer =new char[m_SeedFileSize];
	file.Read((void*) pBuffer, m_SeedFileSize);

	//ResovleBuffer�������ƶ�ָ�룬���Դ���һ��pBuffer�ĸ��������������ͷ�pBuffer
	pShadowBuffer = pBuffer;

	BC_Node Root;
	try
	{
		Root = ResovleBuffer(&pShadowBuffer);
	}
	catch (int)
	{
		SAFE_RETURN(pBuffer, FALSE);//�����ļ��޷�����
	}

	SAFE_CLEAN(pBuffer);

	//�����ļ��ĸ�һ���Ǹ�BC_DICT����
	if(Root.Type != BC_DICT) return FALSE;

	BC_Dict RootDict, InfoDict;//���ֵ��info�ֵ�
	BC_Node node;

	RootDict = Root.Data.bcDict;
	//������Ӳ�����KEYWORD_INFO�ؼ��ֻ��߲��Ǹ��ֵ�����Ϊ��������Ч
	if(GetNode(RootDict, KEYWORD_INFO, node) &&
		node.Type == BC_DICT)
		InfoDict = node.Data.bcDict;
	else
		return FALSE;

	/* ��ʼȡֵ */

	//������ȡ�����ӵı����ʽ
	if(GetNode(RootDict, KEYWORD_ENCODING, node) &&
		node.Type == BC_STRING)
	{
		if(!strlen(node.Data.bcString) || strcmp(node.Data.bcString, "utf-8") == 0)
			SeedInfo.Seed_Encoding = CP_UTF8;
		else if(strcmp(node.Data.bcString, "utf-7") == 0)//utf-7
			SeedInfo.Seed_Encoding = CP_UTF7;
		else if(strcmp(node.Data.bcString, "gbk") == 0)//��������
			SeedInfo.Seed_Encoding = 936;
		else if(strcmp(node.Data.bcString, "big5") == 0)//��������
			SeedInfo.Seed_Encoding = 950;
		else if(strcmp(node.Data.bcString, "shift_jis") == 0)//����
			SeedInfo.Seed_Encoding = 932;
		else if(strcmp(node.Data.bcString, "windows-874") == 0)//̩��
			SeedInfo.Seed_Encoding = 874;
		else if(strcmp(node.Data.bcString, "ks_c_5601-1987") == 0)//����
			SeedInfo.Seed_Encoding = 949;
		else//������Ϊutf-8
			SeedInfo.Seed_Encoding = CP_UTF8;
	}
	else
		SeedInfo.Seed_Encoding = CP_UTF8;//Ĭ��Ϊutf-8


	//���ӵĴ�������
	if(GetNode(RootDict, KEYWORD_CREATION_DATE, node) &&
		node.Type == BC_INT)
	{
		CTime time(0);
		CTimeSpan ts(node.Data.bcInt);

		time += ts;
		SeedInfo.Seed_CreationDate.Format(_T("%d��%.2d��%.2d�� %.2dʱ%.2d��"), time.GetYear(),
			time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
	}
	else
		SeedInfo.Seed_CreationDate = _T("(δ֪����)");

	//���ӵı�ע
	if(GetNode(RootDict, KEYWORD_COMMENT_UTF8, node) &&//���ȿ���û��utf-8�����
		node.Type == BC_STRING)
	{
		ConvertToUnicode(node.Data.bcString, SeedInfo.Seed_Comment, CP_UTF8);
	}
	else if(GetNode(RootDict, KEYWORD_COMMENT, node) &&//���û��utf-8����ľ�ʹ�����ñ����
		node.Type == BC_STRING)
	{
		ConvertToUnicode(node.Data.bcString, SeedInfo.Seed_Comment);
	}
	else
	{
		SeedInfo.Seed_Comment = _T("(û�б�ע)");
	}

	//���ӵĴ�������
	if(GetNode(RootDict, KEYWORD_CREATED_BY, node) &&
		node.Type == BC_STRING)
	{
		ConvertToUnicode(node.Data.bcString, SeedInfo.Seed_Creator, CP_UTF8);
	}
	else
	{
		SeedInfo.Seed_Creator = _T("(δ֪)");
	}

	//���ӵķ�����
	if(GetNode(InfoDict, KEYWORD_PUBLISHER_UTF8, node) &&//���ȿ���û��utf-8�����
		node.Type == BC_STRING)
	{
		ConvertToUnicode(node.Data.bcString, SeedInfo.Seed_Publisher, CP_UTF8);
	}
	else if(GetNode(InfoDict, KEYWORD_PUBLISHER, node) &&//���û��utf-8����ľ�ʹ�����ñ����
		node.Type == BC_STRING)
	{
		ConvertToUnicode(node.Data.bcString, SeedInfo.Seed_Publisher);
	}
	else
	{
		SeedInfo.Seed_Publisher = _T("(δ֪)");
	}

	//�����ڵ������ļ�
	if(GetNode(InfoDict, KEYWORD_FILES, node) &&
		node.Type == BC_LIST)
	{
		//���Ƕ��ļ�ģʽ
		GetMultiModeFileList(node.Data.bcList, SeedInfo.Seed_Files);
	}
	else
	{
		//���ļ�ģʽ

	}

	return TRUE;
}

BOOL CSeedResolver::IsRangeValid( UINT nOffset /* = 0 */ )
{
	return m_Position + nOffset <= m_SeedFileSize;
}

char* CSeedResolver::GetBCString( char** pBuffer )
{
	char* pszRet = NULL;
	char* pszStrLen;
	UINT nLen = 0;

	//bencode�ַ���������1-9�����ֿ�ʼ
	if(**pBuffer < '1' || **pBuffer >'9')
		return pszRet;

	while(*(*pBuffer + nLen) != BC_STRING_DELIMITER)
	{
		++nLen;
		
		//����Ƿ�Խ��
		if(!IsRangeValid(nLen))	return pszRet;
	}

	//nLenΪbcstringǰ���ı�ʾbcstring���ȵ��ַ�����
	pszStrLen = new char[nLen + 1];
	ZeroMemory(pszStrLen, nLen + 1);
	memcpy((void*) pszStrLen, (void*) *pBuffer, nLen);

	//����Ƿ�Խ��
	if(!IsRangeValid(nLen + 1)) return pszRet;

	//�ƶ�ָ��
	*pBuffer += nLen + 1/* 1Ϊ':' */;
	m_Position += nLen + 1;

	//nLen��ʱΪbcstring�ĳ���
	nLen = atoi(pszStrLen);
	SAFE_CLEAN(pszStrLen);

	//����Ƿ�Խ��
	if(!IsRangeValid(nLen)) return pszRet;

	pszRet = new char[nLen + 1];
	ZeroMemory(pszRet, nLen + 1);
	memcpy((void*) pszRet, (void*) *pBuffer, nLen);

	//�ƶ�ָ��
	*pBuffer += nLen;
	m_Position += nLen;

	if(pszRet) vecAlloc.push_back((INT_PTR) pszRet);

	return pszRet;
}

DWORDLONG CSeedResolver::GetBCInt( char** pBuffer )
{
	DWORDLONG dwlRet = _I64_MAX;
	char* psz;
	UINT nLen = 0;

	//bencode����������i��ʼ
	if(**pBuffer != BC_PREFIX_INT)
		return dwlRet;

	//����Ƿ�Խ��
	if(!IsRangeValid(1)) return dwlRet;

	++(*pBuffer);
	++m_Position;

	while(m_Position <= m_SeedFileSize && *(*pBuffer + nLen) != BC_SUFFIX)
	{
		++nLen;

		//����Ƿ�Խ��
		if(!IsRangeValid(nLen)) return dwlRet;
	}

	psz = new char[nLen + 1];
	ZeroMemory(psz, nLen + 1);
	memcpy((void*) psz, (void*) *pBuffer, nLen);
	if(*psz == '0' && nLen == 1)//0��bencode��Ч������
		dwlRet = 0;
	else
		dwlRet = _atoi64(psz) ? _atoi64(psz) : _I64_MAX;
	SAFE_CLEAN(psz);

	//����Ƿ�Խ��
	if(!IsRangeValid(nLen + 1)) return _I64_MAX;

	//�ƶ�ָ��
	*pBuffer += nLen + 1/* 1Ϊ'e' */;
	m_Position += nLen + 1;

	return dwlRet;
}

BC_Node CSeedResolver::ResovleBuffer( char** pBuffer )
{
	BC_Node Node;

	switch(**pBuffer)
	{
	case '0':case '1':case '2':	case '3':case '4':case '5':	case '6':case '7':case '8':case '9':
		Node.Type = BC_STRING;
		Node.Data.bcString = GetBCString(pBuffer);
		if(!Node.Data.bcString) throw NULL;
		break;
	case BC_PREFIX_INT:
		Node.Type = BC_INT;
		Node.Data.bcInt = GetBCInt(pBuffer);
		if(Node.Data.bcInt == _I64_MAX) throw NULL;
		break;
	case BC_PREFIX_LIST:
		Node.Type = BC_LIST;

		//����BC_PREFIX_LIST
		++(*pBuffer);
		++m_Position;

		while(**pBuffer != BC_SUFFIX)
			Node.Data.bcList.push_back(ResovleBuffer(pBuffer));

		//��β��BC_SUFFIX
		++(*pBuffer);
		++m_Position;

		break;
	case BC_PREFIX_DICT:
		Node.Type = BC_DICT;

		//����BC_PREFIX_DICT
		++(*pBuffer);
		++m_Position;

		while(**pBuffer != BC_SUFFIX)
		{
			char* key = GetBCString(pBuffer);
			if(!key) throw NULL;
			Node.Data.bcDict[key] = ResovleBuffer(pBuffer);
		}

		//��β��BC_SUFFIX
		++(*pBuffer);
		++m_Position;

		break;
	default:
		break;
	}

	return Node;
}

void CSeedResolver::DeallocAll()
{
	for(vector<INT_PTR>::iterator iter = vecAlloc.begin();
		iter != vecAlloc.end();
		++iter)
	{
		delete (void*) *iter;
	}
}

BOOL CSeedResolver::GetNode( const BC_Dict& Dict, char* pszKey, BC_Node& node )
{
	BC_Dict::const_iterator iter = Dict.find(pszKey);
	
	if(iter == Dict.end()) return FALSE;

	node = iter->second;

	return TRUE;
}

BOOL CSeedResolver::GetMultiModeFileList( BC_List FileList, vector<InnerFile>& vec )
{
	for(BC_List::iterator iter = FileList.begin();
		iter != FileList.end();
		++iter)
	{
		if(iter->Type != BC_DICT) return FALSE;//files�б���Ӧ�����ֵ�����

		InnerFile iFile;
		BC_Node node;
		int nCodePage = 0;

		if(GetNode(iter->Data.bcDict, KEYWORD_PATH_UTF8, node))
			nCodePage = CP_UTF8;
		else if(GetNode(iter->Data.bcDict, KEYWORD_PATH, node))
			nCodePage = 0;
		else
			return FALSE;

		if(node.Type != BC_LIST || node.Data.bcList.empty()) return FALSE;
		BC_List PathList = node.Data.bcList;

		/* �ļ�������path�б�����һ����Ŀ�������BitComet����Ƕ�ļ������� */
		if(PathList.back().Type != BC_STRING) return FALSE;
		ConvertToUnicode(PathList.back().Data.bcString, iFile.FileName, nCodePage);
		PathList.pop_back();//��ջ
		if(iFile.FileName.Find(_T(BITCOMET_PADDING_FILE_PREFIX)) == 0) continue;

		/* Ŀ¼�� */
		for(BC_List::iterator iter1 = PathList.begin();
			iter1 != PathList.end();
			++iter1)
		{
			if(iter1->Type != BC_STRING) return FALSE;

			CString s;
			ConvertToUnicode(iter1->Data.bcString, s, nCodePage);

			iFile.PathName += s + _T("\\");
		}

		//�ļ���С
		if(GetNode(iter->Data.bcDict, KEYWORD_LENGTH, node) &&
			node.Type == BC_INT)
		{
			iFile.FileSize = node.Data.bcInt;
		}
		else
			iFile.FileSize = 0;

		vec.push_back(iFile);
	}

	return TRUE;
}


void CSeedResolver::ConvertToUnicode( const char* pStr , CString& s, int nCodePage/*= 0 */)
{
	wchar_t* szUnicode = NULL;
	UINT nLen;
	int nCP;
	
	if(!pStr) return;

	/* ���ַ��������ã�Ӧ���Զ������룬�Ҷ��ַ��ı�����벻�죬ֻ������*/
	if(!nCodePage)//û��ָ������ҳ��ʹ�������ڶ���Ĵ���ҳ
		nCP = SeedInfo.Seed_Encoding;
	else
		nCP = nCodePage;

	nLen = MultiByteToWideChar(nCP, NULL, pStr, strlen(pStr) + 1, NULL, NULL);
	szUnicode = new wchar_t[nLen];
	MultiByteToWideChar(nCP, NULL, pStr, strlen(pStr) + 1, szUnicode, nLen);
	s.Format(_T("%s"), szUnicode);
	SAFE_CLEAN(szUnicode);
}

char* CSeedResolver::ReadSeedFile()
{
	char* pBuffer = NULL;

	return pBuffer;
}

BOOL CSeedResolver::ParseBecodeDic(const std::string& BencodeString)
{
	char* pShadowBuffer = (char*)BencodeString.c_str();

	BC_Node Root;
	try
	{
		Root = ResovleBuffer(&pShadowBuffer);
	}
	catch (int)
	{
		return FALSE;
	}

	//�����ļ��ĸ�һ���Ǹ�BC_DICT����
	if(Root.Type != BC_DICT) return FALSE;

	BC_Dict RootDict, InfoDict;//���ֵ��info�ֵ�
	BC_Node node;

	RootDict = Root.Data.bcDict;
	//������Ӳ�����KEYWORD_INFO�ؼ��ֻ��߲��Ǹ��ֵ�����Ϊ��������Ч
	if(GetNode(RootDict, KEYWORD_INFO, node) &&
		node.Type == BC_DICT)
		InfoDict = node.Data.bcDict;
	else
		return FALSE;

	/* ��ʼȡֵ */

	//������ȡ�����ӵı����ʽ
	if(GetNode(RootDict, KEYWORD_ENCODING, node) &&
		node.Type == BC_STRING)
	{
		if(!strlen(node.Data.bcString) || strcmp(node.Data.bcString, "utf-8") == 0)
			SeedInfo.Seed_Encoding = CP_UTF8;
		else if(strcmp(node.Data.bcString, "utf-7") == 0)//utf-7
			SeedInfo.Seed_Encoding = CP_UTF7;
		else if(strcmp(node.Data.bcString, "gbk") == 0)//��������
			SeedInfo.Seed_Encoding = 936;
		else if(strcmp(node.Data.bcString, "big5") == 0)//��������
			SeedInfo.Seed_Encoding = 950;
		else if(strcmp(node.Data.bcString, "shift_jis") == 0)//����
			SeedInfo.Seed_Encoding = 932;
		else if(strcmp(node.Data.bcString, "windows-874") == 0)//̩��
			SeedInfo.Seed_Encoding = 874;
		else if(strcmp(node.Data.bcString, "ks_c_5601-1987") == 0)//����
			SeedInfo.Seed_Encoding = 949;
		else//������Ϊutf-8
			SeedInfo.Seed_Encoding = CP_UTF8;
	}
	else
		SeedInfo.Seed_Encoding = CP_UTF8;//Ĭ��Ϊutf-8


	//���ӵĴ�������
	if(GetNode(RootDict, KEYWORD_CREATION_DATE, node) &&
		node.Type == BC_INT)
	{
		CTime time(0);
		CTimeSpan ts(node.Data.bcInt);

		time += ts;
		SeedInfo.Seed_CreationDate.Format(_T("%d��%.2d��%.2d�� %.2dʱ%.2d��"), time.GetYear(),
			time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
	}
	else
		SeedInfo.Seed_CreationDate = _T("(δ֪����)");

	//���ӵı�ע
	if(GetNode(RootDict, KEYWORD_COMMENT_UTF8, node) &&//���ȿ���û��utf-8�����
		node.Type == BC_STRING)
	{
		ConvertToUnicode(node.Data.bcString, SeedInfo.Seed_Comment, CP_UTF8);
	}
	else if(GetNode(RootDict, KEYWORD_COMMENT, node) &&//���û��utf-8����ľ�ʹ�����ñ����
		node.Type == BC_STRING)
	{
		ConvertToUnicode(node.Data.bcString, SeedInfo.Seed_Comment);
	}
	else
	{
		SeedInfo.Seed_Comment = _T("(û�б�ע)");
	}

	//���ӵĴ�������
	if(GetNode(RootDict, KEYWORD_CREATED_BY, node) &&
		node.Type == BC_STRING)
	{
		ConvertToUnicode(node.Data.bcString, SeedInfo.Seed_Creator, CP_UTF8);
	}
	else
	{
		SeedInfo.Seed_Creator = _T("(δ֪)");
	}

	//���ӵķ�����
	if(GetNode(InfoDict, KEYWORD_PUBLISHER_UTF8, node) &&//���ȿ���û��utf-8�����
		node.Type == BC_STRING)
	{
		ConvertToUnicode(node.Data.bcString, SeedInfo.Seed_Publisher, CP_UTF8);
	}
	else if(GetNode(InfoDict, KEYWORD_PUBLISHER, node) &&//���û��utf-8����ľ�ʹ�����ñ����
		node.Type == BC_STRING)
	{
		ConvertToUnicode(node.Data.bcString, SeedInfo.Seed_Publisher);
	}
	else
	{
		SeedInfo.Seed_Publisher = _T("(δ֪)");
	}

	//�����ڵ������ļ�
	if(GetNode(InfoDict, KEYWORD_FILES, node) &&
		node.Type == BC_LIST)
	{
		//���Ƕ��ļ�ģʽ
		GetMultiModeFileList(node.Data.bcList, SeedInfo.Seed_Files);
	}
	else
	{
		//���ļ�ģʽ

	}

	return TRUE;
}
