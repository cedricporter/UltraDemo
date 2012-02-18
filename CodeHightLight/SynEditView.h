

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
	// ����**************************************************************************************
	void setBreakPoint ( int nLineNumber );
	void removeBreakPoint ( int nLineNumber );
	void showLineCode ( int nLineNumber );
	void SelectLine( int nLineNo );
	void UpdateContext( const CString& context );
    // ��������***********************************************************************************
	int m_nHorzMaxOld;
	BOOL m_bAllowDraw;
	void ReCalcCursorPostion();
	void SetSynCtrlTabSize(int nSize);
	int GetSpaceWidth(); // �õ��ո�Ŀ��
	int GetVisibleLineCount();
	void ShowTabSpace(BOOL bShow);

	BOOL IsShowTabSpace();
	BOOL IsShowCross();

	void ShowCross(BOOL bShow);
	void SetAutoWordSelect(BOOL bAutoSelect);
	BOOL IsAutoWordSelect();
	void SetUndoLimits(int nMax); // ���ó����ļ���
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
	COLORREF GetColor(int nColorIndex); // �������õ���ɫ
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
	void InitEditorFont(); // ��ʼ������
	int m_nBookMarksCount;   

	/**************************************************************************************************/
	// ����
	void formatCode( UINT nChar, CHARRANGE cr, int* bHasSpecialChar);
	// �ϵ���λ�ô洢
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
	BOOL IsFileExist(CString &strFile); // �ж��ļ��Ƿ����

	CFont m_font;
	LOGFONT m_lf; // ��ʾ����

	int m_nCharNumberWidth; // ���ֿ�ȣ���SetSynEditViewFont�б������¼���
	int m_nLineHeight; // �иߣ� ��SetSynEditViewFont�б������¼���
	int m_nWrapMode; // �ı����з�ʽ
	int m_nTabSize; // Tab��ȣ��൱�ڼ�������8���, ͨ��SetSynEditViewTabSizeд��
	int m_nTabSizeInPoint; //Tab��ȣ�twip���
	int m_nLeftMargin; // �ͻ�����߾�
	int m_nLanguage; // ��ǰ�������
	CRect m_rcClient; // SynEditView�Ŀͻ�������OnSize�б������¼���
	CBitmap *m_pCacheBitmap; //�ڴ滺�廭��
	COLORREF m_clrBkColor; //����ɫ
	COLORREF m_clrCommentColor; //ע��ɫ
	COLORREF m_clrSyntaxColor; //�﷨ɫ
	COLORREF m_clrNormalColor; //����ɫ	
	COLORREF m_clrStringColor; //�ַ���ɫ
	COLORREF m_clrCharColor; //�ַ�����ɫ
	COLORREF m_clrNumberColor; //����ɫ
	COLORREF m_clrUnderLine; //�»��ߵ���ɫ
	COLORREF m_clrBkCurLine; //��ǰ�еı�������ɫ
	COLORREF m_clrLineEnd; //��ĩ��ͷ��ɫ

	COLORREF m_clrLinNumberBkColor; //�����ʾ�кŵı���ɫ
	COLORREF m_clrLinNumberNormal; //�����ʾ�кŵ���ɫ
	COLORREF m_clrCurLinNumber; //�����ʾ��ǰ�кŵ���ɫ
	COLORREF m_clrLinNumberSep; //�кŷָ�����ɫ
	COLORREF m_clrCross; //ʮ�ּ���ɫ
	COLORREF m_clrBKSelText; //��ѡ���ı��ı���ɫ
	COLORREF m_clrTab; //ʮ�ּ���ɫ

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNEDITVIEW_H__A9D39C50_1DC1_40DB_80F7_ACCD8988A157__INCLUDED_)

