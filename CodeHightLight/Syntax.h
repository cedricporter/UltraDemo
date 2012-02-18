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

//������ɫ���
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

// ѡ�����������
void SetCurLanguage(int nLanguage);

// ���عؼ���
void LoadSynWord(CStringArray &m_strArrayKeyWords, int nLanguage);

// �ж��Ƿ��ǹؼ���
BOOL IsSynWord(CStringArray &m_strArrayKeyWords, CString &strReadyToTest);

// �ж��Ƿ�������
BOOL IsNumber(CString &strReadyToTest);

// �Թؼ�������,�������
void SortSynWordArray(CStringArray &m_strArrayKeyWords);

DWORD ParseLine(CStringArray &m_strArrayKeyWords, DWORD dwCookie, CString &strLine, COLORINFO *ColorInfo, int &nActualItems);
// ��������
DWORD ParseLineForHtml(CStringArray &m_strArrayKeyWords, DWORD dwCookie, CString &strLine, COLORINFO *ColorInfo, int &nActualItems);

// �����﷨����
void LoadParseRule(int nlanguage);

// �õ���Ӧ���Ե�����
CString GetLanguageString(int nlanguage);

// �����½��Ķ���
void DeleteAllocString();

// �õ�ע��
CString GetLineCommentString(int nlanguage);

// �õ����Եı��
int GetLanguageByStringShow(CString strlanguage);

CString GetStringShowByLanguage(int nlanguage);
