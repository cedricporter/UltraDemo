// Syntax.h


enum {
	COLORINDEX_COMMENT,
	COLORINDEX_SYNTAX,
	COLORINDEX_NORMAL,
	COLORINDEX_NUMBER,
	COLORINDEX_STRING,
	COLORINDEX_CHAR,
	COLORINDEX_BK,
	COLORINDEX_UNDERLINE,
	COLORINDEX_LINEEND,
	COLORINDEX_BKCURLINE,
	COLORINDEX_LINNUMBERBKCOLOR,
	COLORINDEX_LINNUMBERNORMAL,
	COLORINDEX_CURLINNUMBER,
	COLORINDEX_LINNUMBERSEP,
	COLORINDEX_CROSS,
	COLORINDEX_TAB,
	COLORINDEX_SPACE,
	COLORINDEX_BKSELTEXT,
};

enum {
	_ADA,
	_AML,
	_ASM,
	_BASIC,
	_BATCH,
	_BLUE,
	_CLIPPER,
	_COBOL,
	_CPP,
	_CSHAPE,
	_DATAFLEX,
	_EIFFEL,
	_FORTRAN,
	_HLBL,
	_HTML,
	_INI,
	_JAVA,
	_JSP,
	_LIMBO,
	_LISP,
	_MODULA2,
	_OBJECTPASCAL,
	_PARADOX,
	_PERL,
	_PHP,
	_PL1,
	_PROGRESS,
	_PYTHON,
	_REBOL,
	_REXX,
	_RUBY,
	_SMALLTALK,
	_SQL,
	_VHDL,
	_XML,
 	NONE,
};


#define COOKIE_COMMENT			0x0002
#define COOKIE_EXT_COMMENT		0x0004
#define COOKIE_STRING			0x0010
#define COOKIE_CHAR				0x0020
#define COOKIE_SCRIPT			0x0040 //For Html Parser
#define COOKIE_TAG				0x0080 //For Html Parser

//定义颜色块宏
#define DEFINE_BLOCK(pos, color)	\
	ASSERT((pos) >= 0 && (pos) <= nLength);\
	if (ColorInfo != NULL)\
{\
	if (nActualItems == 0 || ColorInfo[nActualItems - 1].Pos <= (pos)) {\
		ColorInfo[nActualItems].Pos = (pos);\
		ColorInfo[nActualItems].Color = (color);\
		nActualItems ++;\
	}\
}

struct COLORINFO {
	int Pos;
	int Color;
};

// 选择高亮的语言
void SetCurLanguage(int nLanguage);

// 加载关键字
void LoadSynWord(CStringArray &m_strArrayKeyWords, int nLanguage);

// 判断是否是关键字
BOOL IsSynWord(CStringArray &m_strArrayKeyWords, CString &strReadyToTest);

// 判断是否是数字
BOOL IsNumber(CString &strReadyToTest);

// 对关键字排序,方便查找
void SortSynWordArray(CStringArray &m_strArrayKeyWords);

DWORD ParseLine(CStringArray &m_strArrayKeyWords, DWORD dwCookie, CString &strLine, COLORINFO *ColorInfo, int &nActualItems);
// 可以无视
DWORD ParseLineForHtml(CStringArray &m_strArrayKeyWords, DWORD dwCookie, CString &strLine, COLORINFO *ColorInfo, int &nActualItems);

// 加载语法规则
void LoadParseRule(int nlanguage);

// 得到相应语言的名字
CString GetLanguageString(int nlanguage);

// 回收新建的东西
void DeleteAllocString();

// 得到注释
CString GetLineCommentString(int nlanguage);

// 得到语言的标记
int GetLanguageByStringShow(CString strlanguage);

CString GetStringShowByLanguage(int nlanguage);
