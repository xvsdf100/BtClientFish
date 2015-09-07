#pragma once

#include <map>
#include <vector>

#define SAFE_CLEAN(p) if(p) delete p;p = NULL;
#define SAFE_RETURN(p, ret) { SAFE_CLEAN(p);return ret; }

/* bencode的前后缀还有string的分隔符 */
#define BC_PREFIX_INT			'i'
#define BC_PREFIX_LIST			'l'
#define BC_PREFIX_DICT			'd'
#define BC_SUFFIX				'e'
#define BC_STRING_DELIMITER		':'

/* 种子内BitComet的内嵌文件前缀，忽略这些文件 */
#define BITCOMET_PADDING_FILE_PREFIX "_____padding_file"

/* 种子文件的关键字，种子文件整个是一个bencode字典类型，即这个字典的关键字，除非有特别说明，否则所有的字符串值都是utf-8编码 */
#define KEYWORD_ANNOUNCE		"announce"		//bencode字符串
#define KEYWORD_ANNOUNCE_LIST	"announce-list"	//bencode列表的列表，非必要关键字，即announce-list列表内仍是多个列表，每个列表包含一个或多个url字符串
#define KEYWORD_CREATION_DATE	"creation date"	//bencode整数，非必要关键字，种子的创建时间,是自1970-1-1 00:00:00 UTC 所经过的秒数
#define KEYWORD_COMMENT			"comment"		//bencode字符串，非必要关键字，种子文件的备注
#define KEYWORD_COMMENT_UTF8	"comment.utf-8"	//bencode字符串,这个关键字我从官方查不到，可能是BitComet扩展的关键字，如果存在这个关键字就忽略上面那个，因为这个一定是UTF-8编码的
#define KEYWORD_CREATED_BY		"created by"	//bencode字符串，非必要关键字，创建该种子文件的软件和其版本，形式是这样 uTorrent/183B，表示uTorrent 183B版创建
#define KEYWORD_ENCODING		"encoding"		/*bencode字符串，非必要关键字，官方解释是种子文件info字典内pieces段的字符串所使用的编码方式，
												 *但实际发现该处定义的编码方式同样会影响info字典内的所有字符串，不知是什么情况 */
#define KEYWORD_PUBLISHER		"publisher"		//bencode字符串，种子的发布者，这个关键字我从官方也查不到
#define KEYWORD_PUBLISHER_UTF8	"publisher.utf-8"
#define KEYWORD_INFO			"info"			//bencode字典，包含种子内的所有文件，具体看说明http://wiki.theory.org/BitTorrentSpecification

/* 下面这些关键字是info字典段的关键字 */
#define KEYWORD_INFO_PIECE_LENGTH	"piece length"	/* bencode整数，每一块的长度，单位字节，官方建议是524288即512k字节，但可能有不同。*/

#define KEYWORD_INFO_PIECES			"pieces"		/* bencode字符串，每个块的哈希值（SHA1，固定20字节长）的组合，所以这个字符串可能很长，
													 * 官方说明“byte string, i.e. not urlencoded”，显然这个串的长度必须是20的倍数，也可通过
													 * 这个串的长度除以20计算一共有多少块。
													 * 注意，这个字符串并不是标准的c字符串，它的任何位置都可能是null字符，字符串函数不能适用于该字符串 */

#define KEYWORD_INFO_PRIVATE		"private"		/* bencode整数，非必要关键字，实际这个可以看成是一个BOOL，当为1时，表示只通过种子内的服务器得到peers，
													 * 当为0或未设置时，可以从外部获得peers，例如DHT网络 */

/* 以下关键字是和文件相关的，具体看说明http://wiki.theory.org/BitTorrentSpecification */
#define KEYWORD_NAME		"name"
#define KEYWORD_NAME_UTF8	"name.utf-8"//同KEYWORD_COMMENT_UTF8
#define KEYWORD_FILES		"files"
#define KEYWORD_PATH		"path"
#define KEYWORD_PATH_UTF8	"path.utf-8"//同KEYWORD_COMMENT_UTF8
#define KEYWORD_LENGTH		"length"

/* bencode的类型名称 */
enum BC_TYPE_NAME { BC_STRING, BC_INT, BC_LIST, BC_DICT };

struct _BC_Node;//树的节点声明，定义在下面

/* 仿函数 */
struct CompareKey
{
	BOOL operator() (const char* psz1, const char* psz2)
	{
		return strcmp(psz1, psz2) < 0;//严格弱排序，只有在psz1小于psz2时才返回TRUE
	}
};

/* bencode的类型 */
typedef char*									BC_String;
typedef DWORDLONG								BC_Int;
typedef std::map<char*, _BC_Node, CompareKey>	BC_Dict;
typedef std::vector<_BC_Node>					BC_List;

/* 树节点定义，看起来和一般树节点的定义不大一样，参看上面的
 * BC_Dict和BC_List的声明，实际这仍是一个递归定义 */
typedef struct _BC_Node
{
	UINT Type;

	/* 这里实际上使用union最合适，但map和vector有构造/析构函数，
	 * 无法定义为union成员。所以访问节点数据时一定要首先判断
	 * 节点的类型，防止内存读写违例 */
	struct _Data 
	{
		BC_String	bcString;
		BC_Int		bcInt;
		BC_List		bcList;
		BC_Dict		bcDict;
	} Data;

} BC_Node, *LPBC_Node;






/* 种子内的文件 */
typedef struct _Inner_File
{
	/* 路径名 */
	CString PathName;

	/* 文件名 */
	CString FileName;

	/* 文件大小，单位是字节 */
	DWORDLONG FileSize;

	/* 文件类型，种子文件不直接提供，自己解析，见BT FileResolverView.cpp文件中定义的GetFileInfo函数 */
	CString FileType;

	_Inner_File() { FileSize = 0; }
} InnerFile, *PInnerFile;

/* 这个结构包含本程序要用到的所有数据，种子内的其他数据用不到。
 * 其实解析过程还可以优化，直接跳过不需要的种子数据，但没有实现，
 * 一是懒，二是以后有可能用到，所以还是保留完全解析 */
typedef struct _Values_Needed
{
	CString Seed_FileName;
	CString Seed_CreationDate;
	CString Seed_Comment;
	CString Seed_Creator;
	CString Seed_Publisher;
	int Seed_Encoding;//CodePage

	std::vector<InnerFile> Seed_Files;//种子内所有文件的列表
} Values_Needed;

class CSeedResolver
{
private:
	UINT m_SeedFileSize;
	UINT m_Position;
	BC_Dict m_SeedDict;//种子的根，是个bencode字典类型
	std::vector<INT_PTR> vecAlloc;//保存所有已分配内存的指针
	bool m_isBencodeString;//是否字符串

	/* 读取种子文件 */
	char* ReadSeedFile();

	/* 检查是否越界(超过文件结尾) */
	BOOL IsRangeValid(UINT nOffset = 0);

	/* 取得一个bencode字符串(标准c字符串) */
	char* GetBCString(char** pBuffer);

	/* 取得一个bencode整数 */
	DWORDLONG GetBCInt(char** pBuffer);

	/* 解析指定的缓冲区，这是个递归函数 */
	BC_Node ResovleBuffer(char** pBuffer);

	/* 得到一个节点，返回值为该节点是否存在 */
	BOOL GetNode(const BC_Dict& Dict, char* pszKey, BC_Node& node);

	/* 得到种子内的文件列表，返回值为是否成功 */
	BOOL GetMultiModeFileList(BC_List FileList, std::vector<InnerFile>& vec);

	/* 解析一个path列表的 */

	/* 释放解析过程中分配的所有内存 */
	void DeallocAll();

	/* 标准c字符串 -> Unicode，nCodePage = 0表示使用种子内定义的代码页 */
	void ConvertToUnicode( const char* pStr ,CString& s, int nCodePage = 0);

public:
	Values_Needed SeedInfo;

	CSeedResolver(const CString& SeedFileName);
	~CSeedResolver(void);

	/* 初始化后必须首先调用该函数解析，根据返回值确定是否解析成功 */
	BOOL Resolve();

	//增加字符串解析
	BOOL ParseBecodeDic(cons std::string& BencodeString);


};
