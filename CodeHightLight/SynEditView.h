

#if !defined(AFX_SYNEDITVIEW_H__A9D39C50_1DC1_40DB_80F7_ACCD8988A157__INCLUDED_)
#define AFX_SYNEDITVIEW_H__A9D39C50_1DC1_40DB_80F7_ACCD8988A157__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxrich.h"
#include <list>

#define WM_SHOWLINE			(WM_USER + 100)
#define WM_UPDATECONTEXT	(WM_USER + 101)


class CSynEditView : public CRichEditView
{
protected: // create from serialization only
	CSynEditView();
	DECLARE_DYNCREATE(CSynEditView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSynEditView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	// 新增**************************************************************************************
	void setBreakPoint ( int nLineNumber );
	void removeBreakPoint ( int nLineNumber );
	void showLineCode ( int nLineNumber );
	void SelectLine( int nLineNo );
	void UpdateContext( const CString& context );
    // 新增结束***********************************************************************************
	int m_nHorzMaxOld;
	BOOL m_bAllowDraw;
	void ReCalcCursorPostion();
	void SetSynCtrlTabSize(int nSize);
	int GetSpaceWidth(); // 得到空格的宽度
	int GetVisibleLineCount();
	void ShowTabSpace(BOOL bShow);

	BOOL IsShowTabSpace();
	BOOL IsShowCross();

	void ShowCross(BOOL bShow);
	void SetAutoWordSelect(BOOL bAutoSelect);
	BOOL IsAutoWordSelect();
	void SetUndoLimits(int nMax); // 设置撤销的极限
	int GetUndoLimits();
	const std::list<int>& GetBreakPointList()	{ return breakPoint; }
	void SetParseCookieZeroFromGivenLine(int nParseArraySize);
	void ReadColorInfo(int nLanguage);
	void SetReadOnly(BOOL bReadOnly);
	BOOL IsReadOnly();
	void GetSelLine(int &nfirstline, int &nendline);
	CString GetSingleCommentString(int nlanguage);
	BOOL GetLineString(int nLine, CString &strLine);
	int GetTabSize();
	BOOL IsShowUnderLine();
	BOOL IsShowReturnLineToken(); // ????
	BOOL IsShowSelectLine();
	BOOL IsShowLineNumber();
	COLORREF GetColor(int nColorIndex); // 由索引得到颜色
	void SetColor(int nColorIndex, COLORREF clrNew);
	void ReadSettings();
	void WriteSettings();
	int GetLanguage();
	LOGFONT GetSynEditViewFont();
	void ShowUnderLine(BOOL bShowUnderLine);
	void ShowReturnLineToken(BOOL bShowReturnLineToken);
	void ShowLineNumber(BOOL bShow);
	void GotoLine(int nLineIndex);
	void ShowSelectLine(BOOL bShowSelectLine);
	void SelectLanguage(int nLanguage);
	int GetWrapMode(); // ????
	void SetWrapMode(int nWrapMode);// ????
	void LoadText(CString &strText);
	void SetSynEditViewFont(LOGFONT lf);
	void LoadFile(CString strFile);
	virtual ~CSynEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSynEditView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nHorzPos;
	int m_nHorzMax;
	void InitEditorFont(); // 初始化字体
	int m_nBookMarksCount;   

	/**************************************************************************************************/
	// 新增
	void formatCode( UINT nChar, CHARRANGE cr, int* bHasSpecialChar);
	// 断点行位置存储
	std::list<int> breakPoint;
	/**************************************************************************************************/


	int anBookMarks[256]; 

	DWORD CalcCookieBackGround(LPVOID arg); 
	BOOL m_bShowTab;
	int m_nLineNumberCharNumber;
	BOOL m_bShowCross;
	void DrawCross(CDC *cacheDC);
	BOOL m_bAutoWordSelect;
	int m_nUndoLimits;
	BOOL m_bReadOnly;
	BOOL m_bShowReturnLineToken;
	void DrawReturnLineToken(BOOL bRealReturn, CDC *cacheDC, CRect &rcline);
	CStringArray m_strArrayKeyWords;
	BOOL IsSelectionInStr(int nFromHere, int nStrWidth, int nTop);
	int m_nLineCount;
	void DrawSynEditView( int nShowLine = -1 );
	BOOL m_bTrack;
	void CalcHorzScrollPos();
	void JudgeInSeletioAndDrawText(CDC *cacheDC, CRect &rcLine, CString &str, COLORREF clrBkColor, COLORREF clrText);
	BOOL IsStrInSelection(int ptLeft, int ptTop, BOOL bleft);
	CPoint m_ptEnd;
	CPoint m_ptStart;
	void GetSelection();
	int m_nCharTabWidth;
	int m_nCharSpaceWidth;
	int m_nDefaultLeftMargin;
	BOOL m_bShowLineNumber;
	BOOL m_bRealReturn;
	void ResetParseCookie();
	BOOL m_bShowSelectLine;
	BOOL m_bShowUnderLine;
	int m_nCurrentLine;
	int m_nParseArraySize;
	DWORD *m_pdwParseCookies;
	DWORD GetParseCookie(int nLineIndex);
	int GetCurrentLine();
	void DrawLineNumber(CDC *cacheDC, int nline, CRect rect);
	void DrawSingleLineColorText(CDC *cacheDC, int nLine, CRect rcLine);
	void SetSynCtrlFont(LOGFONT lf);
	void SetLeftMargin();
	void SetLineHeight();
	void SetCharWidth();
	int GetLinesTop(int nline);
	BOOL IsFileExist(CString &strFile); // 判断文件是否存在

	CFont m_font;
	LOGFONT m_lf; // 显示字体

	int m_nCharNumberWidth; // 数字宽度，在SetSynEditViewFont中必须重新计算
	int m_nLineHeight; // 行高， 在SetSynEditViewFont中必须重新计算
	int m_nWrapMode; // 文本折行方式
	int m_nTabSize; // Tab宽度，相当于几个数字8宽度, 通过SetSynEditViewTabSize写入
	int m_nTabSizeInPoint; //Tab宽度，twip宽度
	int m_nLeftMargin; // 客户区左边距
	int m_nLanguage; // 当前编程语言
	CRect m_rcClient; // SynEditView的客户区，在OnSize中必须重新计算
	CBitmap *m_pCacheBitmap; //内存缓冲画布
	COLORREF m_clrBkColor; //背景色
	COLORREF m_clrCommentColor; //注释色
	COLORREF m_clrSyntaxColor; //语法色
	COLORREF m_clrNormalColor; //正常色	
	COLORREF m_clrStringColor; //字符串色
	COLORREF m_clrCharColor; //字符常量色
	COLORREF m_clrNumberColor; //数字色
	COLORREF m_clrUnderLine; //下划线的颜色
	COLORREF m_clrBkCurLine; //当前行的背景的颜色
	COLORREF m_clrLineEnd; //行末箭头颜色

	COLORREF m_clrLinNumberBkColor; //左边显示行号的背景色
	COLORREF m_clrLinNumberNormal; //左边显示行号的颜色
	COLORREF m_clrCurLinNumber; //左边显示当前行号的颜色
	COLORREF m_clrLinNumberSep; //行号分隔线颜色
	COLORREF m_clrCross; //十字架颜色
	COLORREF m_clrBKSelText; //被选定文本的背景色
	COLORREF m_clrTab; //十字架颜色

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNEDITVIEW_H__A9D39C50_1DC1_40DB_80F7_ACCD8988A157__INCLUDED_)

