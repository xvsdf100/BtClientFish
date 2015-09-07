#pragma once

#include <map>
#include <vector>

#define SAFE_CLEAN(p) if(p) delete p;p = NULL;
#define SAFE_RETURN(p, ret) { SAFE_CLEAN(p);return ret; }

/* bencode��ǰ��׺����string�ķָ��� */
#define BC_PREFIX_INT			'i'
#define BC_PREFIX_LIST			'l'
#define BC_PREFIX_DICT			'd'
#define BC_SUFFIX				'e'
#define BC_STRING_DELIMITER		':'

/* ������BitComet����Ƕ�ļ�ǰ׺��������Щ�ļ� */
#define BITCOMET_PADDING_FILE_PREFIX "_____padding_file"

/* �����ļ��Ĺؼ��֣������ļ�������һ��bencode�ֵ����ͣ�������ֵ�Ĺؼ��֣��������ر�˵�����������е��ַ���ֵ����utf-8���� */
#define KEYWORD_ANNOUNCE		"announce"		//bencode�ַ���
#define KEYWORD_ANNOUNCE_LIST	"announce-list"	//bencode�б���б��Ǳ�Ҫ�ؼ��֣���announce-list�б������Ƕ���б�ÿ���б����һ������url�ַ���
#define KEYWORD_CREATION_DATE	"creation date"	//bencode�������Ǳ�Ҫ�ؼ��֣����ӵĴ���ʱ��,����1970-1-1 00:00:00 UTC ������������
#define KEYWORD_COMMENT			"comment"		//bencode�ַ������Ǳ�Ҫ�ؼ��֣������ļ��ı�ע
#define KEYWORD_COMMENT_UTF8	"comment.utf-8"	//bencode�ַ���,����ؼ����Ҵӹٷ��鲻����������BitComet��չ�Ĺؼ��֣������������ؼ��־ͺ��������Ǹ�����Ϊ���һ����UTF-8�����
#define KEYWORD_CREATED_BY		"created by"	//bencode�ַ������Ǳ�Ҫ�ؼ��֣������������ļ����������汾����ʽ������ uTorrent/183B����ʾuTorrent 183B�洴��
#define KEYWORD_ENCODING		"encoding"		/*bencode�ַ������Ǳ�Ҫ�ؼ��֣��ٷ������������ļ�info�ֵ���pieces�ε��ַ�����ʹ�õı��뷽ʽ��
												 *��ʵ�ʷ��ָô�����ı��뷽ʽͬ����Ӱ��info�ֵ��ڵ������ַ�������֪��ʲô��� */
#define KEYWORD_PUBLISHER		"publisher"		//bencode�ַ��������ӵķ����ߣ�����ؼ����Ҵӹٷ�Ҳ�鲻��
#define KEYWORD_PUBLISHER_UTF8	"publisher.utf-8"
#define KEYWORD_INFO			"info"			//bencode�ֵ䣬���������ڵ������ļ������忴˵��http://wiki.theory.org/BitTorrentSpecification

/* ������Щ�ؼ�����info�ֵ�εĹؼ��� */
#define KEYWORD_INFO_PIECE_LENGTH	"piece length"	/* bencode������ÿһ��ĳ��ȣ���λ�ֽڣ��ٷ�������524288��512k�ֽڣ��������в�ͬ��*/

#define KEYWORD_INFO_PIECES			"pieces"		/* bencode�ַ�����ÿ����Ĺ�ϣֵ��SHA1���̶�20�ֽڳ�������ϣ���������ַ������ܺܳ���
													 * �ٷ�˵����byte string, i.e. not urlencoded������Ȼ������ĳ��ȱ�����20�ı�����Ҳ��ͨ��
													 * ������ĳ��ȳ���20����һ���ж��ٿ顣
													 * ע�⣬����ַ��������Ǳ�׼��c�ַ����������κ�λ�ö�������null�ַ����ַ����������������ڸ��ַ��� */

#define KEYWORD_INFO_PRIVATE		"private"		/* bencode�������Ǳ�Ҫ�ؼ��֣�ʵ��������Կ�����һ��BOOL����Ϊ1ʱ����ʾֻͨ�������ڵķ������õ�peers��
													 * ��Ϊ0��δ����ʱ�����Դ��ⲿ���peers������DHT���� */

/* ���¹ؼ����Ǻ��ļ���صģ����忴˵��http://wiki.theory.org/BitTorrentSpecification */
#define KEYWORD_NAME		"name"
#define KEYWORD_NAME_UTF8	"name.utf-8"//ͬKEYWORD_COMMENT_UTF8
#define KEYWORD_FILES		"files"
#define KEYWORD_PATH		"path"
#define KEYWORD_PATH_UTF8	"path.utf-8"//ͬKEYWORD_COMMENT_UTF8
#define KEYWORD_LENGTH		"length"

/* bencode���������� */
enum BC_TYPE_NAME { BC_STRING, BC_INT, BC_LIST, BC_DICT };

struct _BC_Node;//���Ľڵ�����������������

/* �º��� */
struct CompareKey
{
	BOOL operator() (const char* psz1, const char* psz2)
	{
		return strcmp(psz1, psz2) < 0;//�ϸ�������ֻ����psz1С��psz2ʱ�ŷ���TRUE
	}
};

/* bencode������ */
typedef char*									BC_String;
typedef DWORDLONG								BC_Int;
typedef std::map<char*, _BC_Node, CompareKey>	BC_Dict;
typedef std::vector<_BC_Node>					BC_List;

/* ���ڵ㶨�壬��������һ�����ڵ�Ķ��岻��һ�����ο������
 * BC_Dict��BC_List��������ʵ��������һ���ݹ鶨�� */
typedef struct _BC_Node
{
	UINT Type;

	/* ����ʵ����ʹ��union����ʣ���map��vector�й���/����������
	 * �޷�����Ϊunion��Ա�����Է��ʽڵ�����ʱһ��Ҫ�����ж�
	 * �ڵ�����ͣ���ֹ�ڴ��дΥ�� */
	struct _Data 
	{
		BC_String	bcString;
		BC_Int		bcInt;
		BC_List		bcList;
		BC_Dict		bcDict;
	} Data;

} BC_Node, *LPBC_Node;






/* �����ڵ��ļ� */
typedef struct _Inner_File
{
	/* ·���� */
	CString PathName;

	/* �ļ��� */
	CString FileName;

	/* �ļ���С����λ���ֽ� */
	DWORDLONG FileSize;

	/* �ļ����ͣ������ļ���ֱ���ṩ���Լ���������BT FileResolverView.cpp�ļ��ж����GetFileInfo���� */
	CString FileType;

	_Inner_File() { FileSize = 0; }
} InnerFile, *PInnerFile;

/* ����ṹ����������Ҫ�õ����������ݣ������ڵ����������ò�����
 * ��ʵ�������̻������Ż���ֱ����������Ҫ���������ݣ���û��ʵ�֣�
 * һ�����������Ժ��п����õ������Ի��Ǳ�����ȫ���� */
typedef struct _Values_Needed
{
	CString Seed_FileName;
	CString Seed_CreationDate;
	CString Seed_Comment;
	CString Seed_Creator;
	CString Seed_Publisher;
	int Seed_Encoding;//CodePage

	std::vector<InnerFile> Seed_Files;//�����������ļ����б�
} Values_Needed;

class CSeedResolver
{
private:
	UINT m_SeedFileSize;
	UINT m_Position;
	BC_Dict m_SeedDict;//���ӵĸ����Ǹ�bencode�ֵ�����
	std::vector<INT_PTR> vecAlloc;//���������ѷ����ڴ��ָ��
	bool m_isBencodeString;//�Ƿ��ַ���

	/* ��ȡ�����ļ� */
	char* ReadSeedFile();

	/* ����Ƿ�Խ��(�����ļ���β) */
	BOOL IsRangeValid(UINT nOffset = 0);

	/* ȡ��һ��bencode�ַ���(��׼c�ַ���) */
	char* GetBCString(char** pBuffer);

	/* ȡ��һ��bencode���� */
	DWORDLONG GetBCInt(char** pBuffer);

	/* ����ָ���Ļ����������Ǹ��ݹ麯�� */
	BC_Node ResovleBuffer(char** pBuffer);

	/* �õ�һ���ڵ㣬����ֵΪ�ýڵ��Ƿ���� */
	BOOL GetNode(const BC_Dict& Dict, char* pszKey, BC_Node& node);

	/* �õ������ڵ��ļ��б�����ֵΪ�Ƿ�ɹ� */
	BOOL GetMultiModeFileList(BC_List FileList, std::vector<InnerFile>& vec);

	/* ����һ��path�б�� */

	/* �ͷŽ��������з���������ڴ� */
	void DeallocAll();

	/* ��׼c�ַ��� -> Unicode��nCodePage = 0��ʾʹ�������ڶ���Ĵ���ҳ */
	void ConvertToUnicode( const char* pStr ,CString& s, int nCodePage = 0);

public:
	Values_Needed SeedInfo;

	CSeedResolver(const CString& SeedFileName);
	~CSeedResolver(void);

	/* ��ʼ����������ȵ��øú������������ݷ���ֵȷ���Ƿ�����ɹ� */
	BOOL Resolve();

	//�����ַ�������
	BOOL ParseBecodeDic(cons std::string& BencodeString);


};
