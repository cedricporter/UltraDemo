#include "stdafx.h"
#include "syntax.h"
#include <malloc.h>
#include "SynEditView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD CALLBACK EditStreamCallbackReadFromFile(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
DWORD CALLBACK EditStreamCallbackWriteToFile(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
DWORD CALLBACK EditStreamCallbackOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

/////////////////////////////////////////////////////////////////////////////
// CSynEditView

IMPLEMENT_DYNCREATE(CSynEditView, CRichEditView)

BEGIN_MESSAGE_MAP(CSynEditView, CRichEditView)
//{{AFX_MSG_MAP(CSynEditView)
ON_WM_SIZE()
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_HSCROLL()
ON_WM_KEYDOWN()
ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSynEditView construction/destruction

CSynEditView::CSynEditView()
{
	ReadSettings();	

	m_nLineNumberCharNumber = 0;
	m_nLineCount = 0; // 行数
	m_nCharTabWidth = 1; 
	m_nCharSpaceWidth = 1;
	m_nParseArraySize = 0 ;// 分析得到的数组大小
	m_nHorzPos = 0;
	m_nCharNumberWidth = 1;
	m_nLineHeight = 17;
	m_nLeftMargin = 1;
	m_nCurrentLine = 0;
	m_bAllowDraw = TRUE;
	m_bRealReturn = TRUE;
	m_nDefaultLeftMargin = 0;

	m_nBookMarksCount = 0;

	m_bTrack = FALSE;
	m_pdwParseCookies = NULL;
	m_rcClient = NULL;
	m_pCacheBitmap = NULL;

	memset(anBookMarks, 0, sizeof(int)*256);

	m_nHorzMaxOld = 0;

}

CSynEditView::~CSynEditView()
{
	if (m_pCacheBitmap != NULL)
		delete m_pCacheBitmap;

	if (m_pdwParseCookies != NULL)
		delete m_pdwParseCookies;
}

BOOL CSynEditView::PreCreateWindow(CREATESTRUCT& cs)
{
//*
	//if (LoadLibraryA("RICHED20.DLL") == NULL)
	//{
	//	AfxMessageBox(_T("Fail to load \"riched20.dll\"."),
	//		MB_OK | MB_ICONERROR);
	//	PostMessage(WM_QUIT,0,0);
	//}
	m_strClass = RICHEDIT_CLASSA;	//for 2.0 class 	
//*/
	return CRichEditView::PreCreateWindow(cs);
}

void CSynEditView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(1000, 800, 800, 800));

	ResetParseCookie();
	
	SelectLanguage(m_nLanguage);

	GetRichEditCtrl().HideSelection(TRUE, FALSE);
	SetWrapMode(m_nWrapMode);// 折行模式设置
	GetRichEditCtrl().SetReadOnly(m_bReadOnly); 

	SendMessage(EM_SETUNDOLIMIT, m_nUndoLimits, 0); // 设置可以撤销的次数
  	SetAutoWordSelect(m_bAutoWordSelect); // ???
	
	CSynEditView::m_nWordWrap = m_nWrapMode;
	CSynEditView::WrapChanged(); 

	ResetParseCookie();

	if(m_nWrapMode == WrapNone)
		GetRichEditCtrl().ShowScrollBar(SB_HORZ, TRUE); 
	else
		GetRichEditCtrl().ShowScrollBar(SB_HORZ, FALSE); 

	InitEditorFont();

	PostMessage( WM_PAINT, 0, 0 );
}

/////////////////////////////////////////////////////////////////////////////
// CSynEditView printing

BOOL CSynEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


/////////////////////////////////////////////////////////////////////////////
// CSynEditView diagnostics

#ifdef _DEBUG
void CSynEditView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CSynEditView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSynEditView message handlers

void CSynEditView::LoadFile(CString strFile)
{
	SetWindowText(_T(""));
	ResetParseCookie();		
	SetSynEditViewFont(m_lf);

	CRichEditCtrl &SynCtrl = GetRichEditCtrl();	

	CFile* pInputFile = NULL;
	try
	{
		pInputFile = new CFile(strFile, CFile::modeRead | CFile::shareDenyNone);
		
		EDITSTREAM strm;
//		strm.dwCookie = (DWORD) pInputFile;
        strm.dwCookie = reinterpret_cast<DWORD>(pInputFile);
		strm.pfnCallback = EditStreamCallbackReadFromFile;
		
		long lResult = SynCtrl.StreamIn(SF_TEXT, strm);
		
		pInputFile->Close();
	}
	catch (CFileException* pEx)
	{
		pEx->Delete();
	}
	delete pInputFile;	
	SynCtrl.SetModify(FALSE); // ?????????? 
	return;
}
 
//将文件中的内容装入到richeditctrl中，正常工作
DWORD CALLBACK EditStreamCallbackReadFromFile(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
//	CFile* pFile = (CFile*) dwCookie;
	CFile* pFile =  reinterpret_cast<CFile*>(dwCookie);
	ASSERT_KINDOF(CFile, pFile);

	*pcb = pFile->Read(pbBuff, cb);

	return 0;
}

//将richeditctrl中的内容写入到文件中，正常工作
DWORD CALLBACK EditStreamCallbackWriteToFile(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
//	CFile* pFile = (CFile*) dwCookie;
    CFile* pFile = reinterpret_cast<CFile*>(dwCookie);
	
	pFile->Write(pbBuff, cb);
	*pcb = cb;
	
	return 0;
}

BOOL CSynEditView::IsFileExist(CString &strFile)
{
	if(strFile.IsEmpty())
		return FALSE;
	FILE *file;
	if(( fopen_s(&file,strFile, _T("r")))==NULL)
		return FALSE;
	fclose(file);
	return TRUE;
}

void CSynEditView::SetSynEditViewFont(LOGFONT lf)
{
	ResetParseCookie();
	SetSynCtrlFont(lf);
	WriteSettings();
}

void CSynEditView::SetSynCtrlFont(LOGFONT lf)
{
	ShowWindow(SW_HIDE);

	CString str;
	EDITSTREAM stream;
//	stream.dwCookie = (DWORD)&str;
    stream.dwCookie = reinterpret_cast<DWORD>(&str);
	stream.pfnCallback = EditStreamCallbackOut;
	GetRichEditCtrl().StreamOut(SF_TEXT, stream);
	
	BOOL bModify = GetRichEditCtrl().GetModify();

	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf);
	//先设置视的字体
	SetFont(&m_font);
	m_lf = lf;

/*
	SetWindowText((" "));
	GetRichEditCtrl().SetSel(0, 1);
	CHARFORMAT cr = GetCharFormatSelection();
	//设置视的字体可能改变，所以还要取其字体，看真正的结果，用GetFont不管用
	m_lf.lfCharSet = cr.bCharSet;
	m_lf.lfHeight = cr.yHeight/14;
	strcpy(m_lf.lfFaceName, cr.szFaceName);		
//*/

	SetCharWidth();
	SetLineHeight();
	SetLeftMargin();

	WriteSettings();
	// 显示文本信息
	SetWindowText(str);
	GetRichEditCtrl().SetModify(bModify);
	ShowWindow(SW_SHOW); 
}


void CSynEditView::SetSynCtrlTabSize(int nSize)
{
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();	

	CString str;
	EDITSTREAM stream;
//	stream.dwCookie = (DWORD)&str;
	stream.dwCookie = reinterpret_cast<DWORD>(&str);
	stream.pfnCallback = EditStreamCallbackOut;
	SynCtrl.StreamOut(SF_TEXT, stream);

	//因为tab宽度一直是固定的，所以开始就取tab宽度
	//又因为要计算最开始tab相当于几个数字宽，所以下面几行必须放在上面一行之后
	//RichEditView开始时tab宽为一常数，即0.5英寸=720 points
	SynCtrl.SetWindowText(_T("\t")); 
	CPoint p1 = SynCtrl.GetCharPos(0);	
	CPoint p2 = SynCtrl.GetCharPos(1);
	m_nCharTabWidth = p2.x - p1.x; //取tab在字体m_lf下的以point表示的宽度

	//设置TAB间隔
	PARAFORMAT2 pf ;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_TABSTOPS ;
	pf.cTabCount = MAX_TAB_STOPS;
	SynCtrl.GetParaFormat( pf );
	int nSynCtrlTabSize = pf.rgxTabs[0];
	if(nSynCtrlTabSize == 0)
		nSynCtrlTabSize = 720;
	
	/*
	Tab的计算方法:
	1、先取SynCtrl默认的Tab宽度nSynCtrlTabSize(用英寸表示)
	2、计算SynCtrl默认的Tab宽度对应几个数字m_nCharTabWidth / m_nCharNumberWidth
	2、根据以上值计算每个数字的宽度nSynCtrlTabSize / (m_nCharTabWidth / m_nCharNumberWidth)
	3、根据每个数字的宽度计算新的Tab宽度
	*/

	SetWindowText(""); //此行不能删除，不然设置tab宽度会失败

	int nNewTab = int(nSynCtrlTabSize * 1.0 * nSize * m_nCharNumberWidth / m_nCharTabWidth);

	pf.cTabCount = MAX_TAB_STOPS;
	pf.dwMask = PFM_TABSTOPS;
	for(int itab = 0; itab < pf.cTabCount; itab++ )
		pf.rgxTabs[itab] = (itab+1) * nNewTab ;
	SetParaFormat( pf );

	m_nTabSize = nSize;	

	m_nCharTabWidth = nSize * m_nCharNumberWidth;

	WriteSettings();

	SetWindowText(str);
}

void CSynEditView::OnSize(UINT nType, int cx, int cy) 
{
	CRichEditView::OnSize(nType, cx, cy);

	GetClientRect(&m_rcClient);
	
	if (m_pCacheBitmap != NULL)
	{
		delete m_pCacheBitmap;
		m_pCacheBitmap = NULL;
	}

	SetLeftMargin();

	PostMessage(WM_PAINT, 0, 0);
}

void CSynEditView::UpdateContext( const CString& context )
{
	CString* text = new CString( context );
	PostMessage( WM_UPDATECONTEXT, (WPARAM)text, 0 );
}

void CSynEditView::LoadText(CString &strText)
{
	ResetParseCookie();
	
	SetSynEditViewFont(m_lf);

	SetWindowText(strText);	
	GetRichEditCtrl().SetModify( FALSE ); 
}


void CSynEditView::DrawSingleLineColorText( CDC *cacheDC, int nLine, CRect rcLine)
{
	COLORREF clrBkColor = m_clrBkColor;
	CRect rtSrc(rcLine);

	// 显示当前行
	if(m_bShowSelectLine) {
		if(nLine == m_nCurrentLine && m_ptStart == m_ptEnd) 
			clrBkColor = m_clrBkCurLine;
		else
			clrBkColor = m_clrBkColor;
		cacheDC->FillSolidRect(rcLine, clrBkColor);
	}	
	////////////
	
	CFont cf;
	cf.CreateFontIndirect(&m_lf);
	CFont *pOldFont = cacheDC->SelectObject(&cf);
	
	CString  strLine;
	BOOL bRealReturn = GetLineString(nLine, strLine); //取指定行文本，并返回是否为硬回车标志

	int nLength = strLine.GetLength(); 
	
	int nActualItems = 0;
	COLORINFO *ColorInfo = (COLORINFO *)_alloca( sizeof(COLORINFO) * (nLength + 1) );	
	DWORD dwCookie = GetParseCookie(nLine-1);
	CString str;
	if( m_bRealReturn && (m_nLanguage != _HTML) && (m_nLanguage != _XML))
		dwCookie &= COOKIE_EXT_COMMENT; 
	m_bRealReturn = bRealReturn;
	dwCookie = ParseLine(m_strArrayKeyWords, dwCookie, strLine, ColorInfo, nActualItems);
	m_pdwParseCookies[nLine] = dwCookie;

	COLORREF clr;
	CSize sizeContinueStr(0, 0);
	int nlen, nTabCount = 0;

	if (nActualItems > 0)
	{
		ASSERT(ColorInfo[0].Pos >= 0 && ColorInfo[0].Pos <= nLength);
		for (int I = 0; I < nActualItems; I ++)
		{
			nlen = ColorInfo[I + 1].Pos - ColorInfo[I].Pos;
			if( I == nActualItems - 1 )
				str = strLine.Mid(ColorInfo[I].Pos, strLine.GetLength() - ColorInfo[I].Pos);
			else
				str = strLine.Mid(ColorInfo[I].Pos, nlen);

			if(str.IsEmpty())
				continue;

			clr = GetColor(ColorInfo[I].Color);

			int nPos = str.Find(_T("\t"));
			while(-1 != nPos)
			{
				//遇到一个tab字符时，先将前面的字符画出来
				CString strTemp = str.Left(nPos);
				JudgeInSeletioAndDrawText( cacheDC, rcLine, strTemp, clrBkColor, clr );
				
				CSize sizeTemp = cacheDC->GetTextExtent(strTemp);

				nTabCount += 1; //既然找到了tab，那么就加1吧
				//把前面已经输出的字符串宽度换算成相应的tab个数
				nTabCount += int((sizeTemp.cx+sizeContinueStr.cx) / m_nCharTabWidth);

				int nOldLeft = rcLine.left;
				//然后直接定位到下一个tab位置
				rcLine.left = rtSrc.left + nTabCount * m_nCharTabWidth;
				
				//开始画tab////////////////////////
				BOOL bLeftInSel = IsStrInSelection(nOldLeft, rcLine.top, TRUE);
				BOOL bRightInSel = IsStrInSelection(rcLine.left, rcLine.top, FALSE);
				if(bLeftInSel && bRightInSel)
				{
					//如果tab在选择区域内，则要画背景
					CRect rtTab(nOldLeft, rcLine.top, rcLine.left, rcLine.bottom);
					cacheDC->FillSolidRect( &rtTab, m_clrBKSelText ); 
				}
				// 是否显示tab键
				if(m_bShowTab)
				{					
					int nleft = nOldLeft;
					int nCenterY = rcLine.top + (rcLine.bottom-rcLine.top)/2;
					for(int pos=1; pos<4; pos++)
					{
						cacheDC->SetPixel( nleft + pos + 0,  pos + nCenterY - 4, m_clrTab );
						cacheDC->SetPixel( nleft + pos + 3,  pos + nCenterY - 4, m_clrTab );
						cacheDC->SetPixel( nleft + pos + 0, -pos + nCenterY + 2, m_clrTab );
						cacheDC->SetPixel( nleft + pos + 3, -pos + nCenterY + 2, m_clrTab );
					}
				}
				//////////////////////////////////////

				str = str.Right(str.GetLength()-nPos-1);
				nPos = str.Find(_T("\t"));

				//重置输出字符串宽度
				sizeContinueStr.cx = 0;
			}

			JudgeInSeletioAndDrawText( cacheDC, rcLine, str, clrBkColor, clr );
			//保存已输出的字符中宽度
			CSize sizeTemp = cacheDC->GetTextExtent(str);
			sizeContinueStr += sizeTemp;
		}
	}
	
	if(m_bShowReturnLineToken)
		DrawReturnLineToken( m_bRealReturn, cacheDC, rcLine );

		//显示下划线
	if(m_bShowUnderLine) {	
		CPen NewPen(PS_SOLID, 1, m_clrUnderLine);
		CPen *OldPen = cacheDC->SelectObject(&NewPen); 
		cacheDC->MoveTo( rtSrc.left, rtSrc.bottom-1);
		cacheDC->LineTo( rtSrc.right, rtSrc.bottom-1);  
		cacheDC->SelectObject(OldPen); 
	}

	cacheDC->SelectObject(pOldFont); 
}

void CSynEditView::SetWrapMode(int nWrapMode)
{
	ShowWindow(SW_HIDE);

	//m_nCurrentLine = 0;
	//m_nParseArraySize = 0;//改变后需要重新计算
	
	m_nWrapMode = nWrapMode;
	CSynEditView::m_nWordWrap = m_nWrapMode;
	CSynEditView::WrapChanged(); 

	ResetParseCookie();

	ShowWindow(SW_SHOW);	
	if(m_nWrapMode == WrapNone)
		GetRichEditCtrl().ShowScrollBar(SB_HORZ, TRUE); 
	else
		GetRichEditCtrl().ShowScrollBar(SB_HORZ, FALSE); 

	WriteSettings();
	CWinApp *app = AfxGetApp();
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_nWrapMode"), m_nWrapMode);  
}

int CSynEditView::GetWrapMode()
{
	return m_nWrapMode;
}

void CSynEditView::SetLeftMargin()
{
	// 设置行号栏的宽度
	if(m_bShowLineNumber)
		m_nLeftMargin = m_nLineNumberCharNumber * m_nCharNumberWidth + m_nDefaultLeftMargin + 4 ;
	else
		m_nLeftMargin =  m_nDefaultLeftMargin;
	
	GetClientRect(&m_rcClient);
	CRect rect(m_rcClient);
	rect.left = m_nLeftMargin;
	int nRightMargin;
	if(m_bShowReturnLineToken && GetWrapMode()==CSynEditView::WrapToWindow)
		nRightMargin = 12; //为行末标志留出空间
	else
		nRightMargin= 0;

	rect.right -= nRightMargin;
	GetRichEditCtrl().SetRect(&rect); 

	if(GetWrapMode()==CSynEditView::WrapToWindow) {
		ResetParseCookie();
	}
}

int CSynEditView::GetLinesTop(int nline)// 得到行的头部y坐标
{
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();
	if( nline > SynCtrl.GetLineCount()-1 ) {
		int nOffset = SynCtrl.LineIndex( nline-1 );
		CPoint p = SynCtrl.GetCharPos( nOffset );	
		return p.y+m_nLineHeight;
	}
	int nOffset = SynCtrl.LineIndex( nline );
	CPoint p = SynCtrl.GetCharPos( nOffset );	
	return p.y;
}


void CSynEditView::SetLineHeight()
{
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();
	SynCtrl.SetWindowText(_T("\n     "));
	CPoint ps, pe;
	ps = SynCtrl.GetCharPos(0);
	pe = SynCtrl.GetCharPos(3);
	m_nLineHeight = pe.y - ps.y; 
}
// 设置字符宽度和空格宽度
void CSynEditView::SetCharWidth()
{
	CPoint p1, p2;
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();

	SynCtrl.SetWindowText(_T("8"));  //数字宽度
	p1 = SynCtrl.GetCharPos(0);
	p2 = SynCtrl.GetCharPos(1);
	m_nCharNumberWidth = p2.x - p1.x;

	m_nDefaultLeftMargin = 15; //设为15主要是为书签留出空间

	SynCtrl.SetWindowText(_T(" "));  //空格宽度
	p1 = SynCtrl.GetCharPos(0);
	p2 = SynCtrl.GetCharPos(1);
	m_nCharSpaceWidth = p2.x - p1.x;
}

// 选择使用的语种
void CSynEditView::SelectLanguage(int nLanguage)
{
	CWinApp *app = AfxGetApp();
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_nLanguage"), nLanguage);  

	m_nLanguage = nLanguage;
	SetCurLanguage(nLanguage);
	ReadColorInfo( m_nLanguage );
	LoadSynWord( m_strArrayKeyWords, m_nLanguage );

	ResetParseCookie();
	SendMessage( WM_PAINT, 0, 0 );
}


void CSynEditView::DrawLineNumber(CDC *cacheDC, int nline, CRect rect)
{	
	int nHalfHeight = rect.top + rect.Height()/2;
	int nLeft;
	if(m_bShowLineNumber)
	{
		nLeft = rect.right+1;
	}
	else
	{
		nLeft = rect.left-1;
	}

	//画断点
	for ( std::list< int >::iterator iter = breakPoint.begin(); iter != breakPoint.end(); iter++ )
	{   		
		if(nline == (*iter))
		{
			int j;
			for(j=0; j<6; j++) 
				cacheDC->SetPixel(nLeft+7+j, nHalfHeight-j, m_clrCurLinNumber);
			for(j=0; j<6; j++) 
				cacheDC->SetPixel(nLeft+7-j, nHalfHeight+j, m_clrCurLinNumber);
			for(j=0; j<5; j++) 
				cacheDC->SetPixel(nLeft+8, nHalfHeight+j, m_clrCurLinNumber);
			for(j=-1; j<6; j++) 
				cacheDC->SetPixel(nLeft+9, nHalfHeight+j, m_clrCurLinNumber);
			for(j=-2; j<7; j++) 
				cacheDC->SetPixel(nLeft+10, nHalfHeight+j, m_clrCurLinNumber);
			for(j=-3; j<7; j++) 
				cacheDC->SetPixel(nLeft+11, nHalfHeight+j, m_clrCurLinNumber);
			for(j=-2; j<7; j++) 
				cacheDC->SetPixel(nLeft+12, nHalfHeight+j, m_clrCurLinNumber);
			for(j=1; j<6; j++) 
				cacheDC->SetPixel(nLeft+13, nHalfHeight+j, m_clrCurLinNumber);
			for(j=2; j<4; j++) 
				cacheDC->SetPixel(nLeft+14, nHalfHeight+j, m_clrCurLinNumber);
		}
	}

	if(!m_bShowLineNumber)
		return;

	CFont newFont;
	newFont.CreateFontIndirect(&m_lf); 
	CFont *pOldFont = cacheDC->SelectObject(&newFont); 

	CString str, str1;
	str1.Format(_T("%%0%d"), m_nLineNumberCharNumber);
	str.Format(str1+_T("d"), nline+1);

	CSize size = cacheDC->GetTextExtent(str);
	int nOffsetY = (rect.Height()-size.cy)/2;
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();
	if( (nline == m_nCurrentLine) && (m_ptStart == m_ptEnd) ) {
		cacheDC->SetTextColor(m_clrCurLinNumber); // 修改当前行号颜色
		cacheDC->TextOut(rect.left , rect.top+nOffsetY, str);
	}
	else {
		cacheDC->SetTextColor( m_clrLinNumberNormal ); 
		cacheDC->TextOut(rect.left, rect.top+nOffsetY, str);
	}

	cacheDC->SelectObject(pOldFont); 
}


int CSynEditView::GetCurrentLine()
{
	CRichEditCtrl &SynCtrl=GetRichEditCtrl();	
	CHARRANGE cr;
	SynCtrl.GetSel(cr); 
	if( cr.cpMin == cr.cpMax )
		return SynCtrl.LineFromChar( cr.cpMax );
	else
		return m_nCurrentLine;
}

// 词法分析得到结果
DWORD CSynEditView::GetParseCookie(int nLineIndex)
{
	CRichEditCtrl &SynCtrl=GetRichEditCtrl();
	// 如果没有分析结果,则创建容器接收结果
	if (m_pdwParseCookies == NULL)
	{
		m_nParseArraySize = m_nLineCount;
		m_pdwParseCookies = new DWORD[m_nLineCount];
		memset(m_pdwParseCookies, 0xff, m_nLineCount * sizeof(DWORD));
	}

	if (nLineIndex < 0)
		return 0;
	if (m_pdwParseCookies[nLineIndex] != (DWORD) -1)
		return m_pdwParseCookies[nLineIndex];

	register int L = nLineIndex;
	while (L >= 0 && m_pdwParseCookies[L] == (DWORD) -1)
		L --;
	L ++;

	int nBlocks = 0;
	CString strLine;
	BOOL bRealReturn = m_bRealReturn;
	while (L <= nLineIndex)
	{
		DWORD dwCookie = 0;
		if (L > 0) {
			dwCookie = m_pdwParseCookies[L - 1];
			if(bRealReturn && m_nLanguage != _HTML && m_nLanguage != _XML)
				dwCookie &= COOKIE_EXT_COMMENT;
		}
		ASSERT(dwCookie != (DWORD) -1);
		bRealReturn = GetLineString(L, strLine);
		m_pdwParseCookies[L] = ParseLine(m_strArrayKeyWords, dwCookie, strLine, NULL, nBlocks);
		ASSERT(m_pdwParseCookies[L] != (DWORD) -1);
		L ++;
	}
	
	return m_pdwParseCookies[nLineIndex];
}

void CSynEditView::ShowUnderLine(BOOL bShowUnderLine)
{
	m_bShowUnderLine = bShowUnderLine;
	WriteSettings();
}

void CSynEditView::ShowReturnLineToken(BOOL bShowReturnLineToken)
{
	ShowWindow(SW_HIDE);

	m_bShowReturnLineToken = bShowReturnLineToken;
	if(GetWrapMode()==WrapToWindow) {
		SetLeftMargin();
		ResetParseCookie();
	}

	WriteSettings();	
	ShowWindow(SW_SHOW);
	SetFocus();
}

void CSynEditView::ShowSelectLine(BOOL bShowSelectLine)
{
	m_bShowSelectLine = bShowSelectLine;
	WriteSettings();
}

//*
BOOL CSynEditView::GetLineString(int nLine, CString &strLine)
{
	strLine = _T("");
	CRichEditCtrl &SynCtrl=GetRichEditCtrl();
	if(nLine > m_nLineCount - 1 || nLine < 0) 
		return TRUE;// 空白行

	int nTemp = SynCtrl.LineIndex(nLine);
	nTemp = SynCtrl.LineLength(nTemp)*16+32; //缓冲区必须足够大，否则读取中文时可能会有乱码

	TCHAR *achLine = new TCHAR[nTemp];
	int nLen = SynCtrl.GetLine(nLine, achLine, nTemp); 	

	//检查实际读出的字符数，一个中文应换算成两个字符
	int i = 0;
	nTemp = 0;
	while(nTemp != nLen)
	{
		if(IsDBCSLeadByte(achLine[i++]))
			i++;
		++nTemp;
	}
	/////////////////////////////////////////////////

	BOOL bRealReturn;
	achLine[i] = 0;
	
	//检查是不是真的换行
	if(0xd == achLine[i-1]) 
	{
		achLine[--i] = 0;  
		bRealReturn=TRUE;
	}
	else 
	{
		achLine[i] = 0;
		bRealReturn=FALSE;
	}
	////////////////////

	//尾部加一个空格，以送给语法解析器
	lstrcat(achLine, _T(" "));
	
	strLine = achLine;

	if(achLine)
		delete achLine;

	return bRealReturn;
}
//*/

/*
BOOL CSynEditView::GetLineString(int nLine, CString &strLine)
{
	strLine = _T("");
	CRichEditCtrl &SynCtrl=GetRichEditCtrl();
	if(nLine > m_nLineCount - 1 || nLine < 0) 
		return TRUE;
		
	int nTemp = SynCtrl.LineIndex(nLine);
	nTemp = SynCtrl.LineLength(nTemp) * sizeof(WCHAR);
	if(nTemp<=1) {
		strLine=_T(" ");
		return TRUE;
	}
	nTemp += 4 * sizeof(WCHAR);
	int nLen = SynCtrl.GetLine(nLine, strLine.GetBuffer(nTemp), nTemp);  		
	strLine.ReleaseBuffer(); 
	BOOL bRealReturn;
	
	if(nLen > strLine.GetLength())
		nLen = strLine.GetLength();
	if(nLen>=1 && strLine[nLen-1]==0xd ) {
		strLine = strLine.Left(--nLen);  
		bRealReturn=TRUE;
	}
	else  {
		strLine = strLine.Left(nLen);  
		bRealReturn=FALSE;
	}
	strLine += _T(" ");

	return bRealReturn;
}
//*/

void CSynEditView::ResetParseCookie()
{
	m_nParseArraySize = 0;
	m_nHorzPos = 0;

	if(m_pdwParseCookies != NULL) {
		delete m_pdwParseCookies;
		m_pdwParseCookies = NULL;
	}

	if(m_pCacheBitmap!=NULL) {
		delete m_pCacheBitmap;
		m_pCacheBitmap = NULL;
	}

	return;
}

// 把光标移动到给定行,行号超出最大值则移动到最后一行
void CSynEditView::GotoLine(int nLine)
{
	CRichEditCtrl &SynCtrl=GetRichEditCtrl();
	m_nLineCount = SynCtrl.GetLineCount(); 
	if(nLine>m_nLineCount)
		nLine = m_nLineCount;

	--nLine;

	int nFirstLine = SynCtrl.GetFirstVisibleLine(); 

	if( nLine < 1)
		nLine = 0;

	int nLineCount = GetVisibleLineCount();
	if(nLine >= nFirstLine && nLine < nFirstLine + nLineCount ) {
		int nLineIndex = SynCtrl.LineIndex(nLine);
		SynCtrl.SetSel(nLineIndex, nLineIndex); 
		PostMessage(WM_PAINT, 0, 0); //可能要重画行号和光标
		return; //如果当前行在视界内则返回
	}

	int nTemp = nLine;
	if(nTemp > m_nLineCount - nLineCount)
		nTemp = m_nLineCount - nLineCount;
	
	int nScrollLines = nTemp - nFirstLine;
	SynCtrl.LineScroll(nScrollLines, 0);
	SetFocus();
}


void CSynEditView::ShowLineNumber(BOOL bShow)
{
	ShowWindow(SW_HIDE);
	m_bShowLineNumber = bShow;	
	SetLeftMargin();

	CRichEditCtrl &SynCtrl = GetRichEditCtrl();	 
	if(bShow)
		SynCtrl.SetBackgroundColor(FALSE, m_clrLinNumberBkColor);
	else
		SynCtrl.SetBackgroundColor(FALSE, m_clrBkColor);

	if(GetWrapMode()==WrapToWindow)
		ResetParseCookie();	
	WriteSettings();
	ShowWindow(SW_SHOW);
}

int CSynEditView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	GetRichEditCtrl().HideSelection(TRUE, FALSE); // 隐藏当前选中的

	return 0;
}


void CSynEditView::OnDestroy() 
{
	WriteSettings();
	DeleteAllocString();
	CRichEditView::OnDestroy();
}


LOGFONT CSynEditView::GetSynEditViewFont()
{
	return m_lf;
}


void CSynEditView::ReadSettings()
{
	CWinApp *app = AfxGetApp();

	m_nWrapMode = app->GetProfileInt(_T("icrEdit_Values"), _T("m_nWrapMode"), WrapNone);  
	m_bShowLineNumber = app->GetProfileInt(_T("icrEdit_Values"), _T("m_bShowLineNumber"), TRUE);  
	m_bShowSelectLine = app->GetProfileInt(_T("icrEdit_Values"), _T("m_bShowSelectLine"), FALSE);  
	m_bShowUnderLine = app->GetProfileInt(_T("icrEdit_Values"), _T("m_bShowUnderLine"), FALSE);  
	m_bShowReturnLineToken = app->GetProfileInt(_T("icrEdit_Values"), _T("m_bShowReturnLineToken"), FALSE);  
	m_bShowCross = app->GetProfileInt(_T("icrEdit_Values"), _T("m_bShowCross"), FALSE);  
	m_bShowTab = app->GetProfileInt(_T("icrEdit_Values"), _T("m_bShowTabSpace"), FALSE);  
	m_nLanguage = app->GetProfileInt(_T("icrEdit_Values"), _T("m_nLanguage"), _CPP);  
	m_nTabSize = app->GetProfileInt(_T("icrEdit_Values"), _T("m_nTabSize"), 4);  
	m_bReadOnly = app->GetProfileInt(_T("icrEdit_Values"), _T("m_bReadOnly"), FALSE);  
	m_bAutoWordSelect = app->GetProfileInt(_T("icrEdit_Values"), _T("m_bAutoWordSelect"), FALSE);  
	m_nUndoLimits = app->GetProfileInt(_T("icrEdit_Values"), _T("m_nUndoLimits"), 100); 
	if(m_nUndoLimits<0)
		m_nUndoLimits=0;

	ReadColorInfo( m_nLanguage );

	//设置icrEditor默认字体
	m_lf.lfHeight = 0xfffffff4;
	m_lf.lfWidth = 0;
	m_lf.lfEscapement = 0;
	m_lf.lfOrientation = 0;
	m_lf.lfWeight = FW_NORMAL;
	m_lf.lfItalic = 0;
	m_lf.lfUnderline = 0;
	m_lf.lfStrikeOut = 0;
	m_lf.lfCharSet = ANSI_CHARSET;
	m_lf.lfOutPrecision = 1;
	m_lf.lfClipPrecision = 1;
	m_lf.lfQuality = 2;
	m_lf.lfPitchAndFamily = 0;
	strcpy_s(m_lf.lfFaceName, _T("Courier New"));

	m_lf.lfHeight = app->GetProfileInt(_T("icrEdit_Font"), _T("lfHeight"), m_lf.lfHeight); 
	m_lf.lfWeight = app->GetProfileInt(_T("icrEdit_Font"), _T("lfWeight"), m_lf.lfWeight);  
	m_lf.lfEscapement = app->GetProfileInt(_T("icrEdit_Font"), _T("lfEscapement"), m_lf.lfEscapement);  
	m_lf.lfOrientation = app->GetProfileInt(_T("icrEdit_Font"), _T("lfOrientation"), m_lf.lfOrientation);  
	m_lf.lfWeight = app->GetProfileInt(_T("icrEdit_Font"), _T("lfWeight"), m_lf.lfWeight);  
	m_lf.lfItalic = app->GetProfileInt(_T("icrEdit_Font"), _T("lfItalic"), m_lf.lfItalic);  
	m_lf.lfUnderline = app->GetProfileInt(_T("icrEdit_Font"), _T("lfUnderline"), m_lf.lfUnderline);  
	m_lf.lfStrikeOut = app->GetProfileInt(_T("icrEdit_Font"), _T("lfStrikeOut"), m_lf.lfStrikeOut);  
	m_lf.lfCharSet = app->GetProfileInt(_T("icrEdit_Font"), _T("lfCharSet"), m_lf.lfCharSet);  
	m_lf.lfOutPrecision = app->GetProfileInt(_T("icrEdit_Font"), _T("lfOutPrecision"), m_lf.lfOutPrecision);  
	m_lf.lfClipPrecision = app->GetProfileInt(_T("icrEdit_Font"), _T("lfClipPrecision"), m_lf.lfClipPrecision);  
	m_lf.lfQuality = app->GetProfileInt(_T("icrEdit_Font"), _T("lfQuality"), m_lf.lfQuality);  
	m_lf.lfPitchAndFamily = app->GetProfileInt(_T("icrEdit_Font"), _T("lfPitchAndFamily"), m_lf.lfPitchAndFamily);  
	CString strFaceName = app->GetProfileString(_T("icrEdit_Font"), _T("lfFaceName"), m_lf.lfFaceName);  
	strcpy_s(m_lf.lfFaceName,strFaceName);
}

void CSynEditView::WriteSettings()
{
	CWinApp *app = AfxGetApp();

	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_nWrapMode"), m_nWrapMode);  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_bShowLineNumber"), m_bShowLineNumber);  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_bShowSelectLine"), m_bShowSelectLine);  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_bShowUnderLine"), m_bShowUnderLine);  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_bShowReturnLineToken"), m_bShowReturnLineToken);  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_bShowCross"), m_bShowCross);  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_bShowTabSpace"), m_bShowTab);  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_nLanguage"), m_nLanguage);  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_nTabSize"), m_nTabSize);  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_bReadOnly"), m_bReadOnly);  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_bAutoWordSelect"), IsAutoWordSelect());  
	app->WriteProfileInt(_T("icrEdit_Values"), _T("m_nUndoLimits"), m_nUndoLimits);  

	CString strColorPos = _T("icrEdit_Color");
	strColorPos += GetLanguageString(m_nLanguage);

	app->WriteProfileInt( strColorPos, _T("m_clrBkColor"), m_clrBkColor);  
	app->WriteProfileInt( strColorPos, _T("m_clrCommentColor"), m_clrCommentColor);  
	app->WriteProfileInt( strColorPos, _T("m_clrSyntaxColor"), m_clrSyntaxColor);  
	app->WriteProfileInt( strColorPos, _T("m_clrNormalColor"), m_clrNormalColor);  
	app->WriteProfileInt( strColorPos, _T("m_clrStringColor"), m_clrStringColor);  
	app->WriteProfileInt( strColorPos, _T("m_clrCharColor"), m_clrCharColor);  
	app->WriteProfileInt( strColorPos, _T("m_clrNumberColor"), m_clrNumberColor);  
	app->WriteProfileInt( strColorPos, _T("m_clrUnderLine"), m_clrUnderLine);  
	app->WriteProfileInt( strColorPos, _T("m_clrBkCurLine"), m_clrBkCurLine);  
	app->WriteProfileInt( strColorPos, _T("m_clrLineEnd"), m_clrLineEnd);  

	app->WriteProfileInt( strColorPos, _T("m_clrLinNumberBkColor"), m_clrLinNumberBkColor);  
	app->WriteProfileInt( strColorPos, _T("m_clrLinNumberNormal"), m_clrLinNumberNormal);  
	app->WriteProfileInt( strColorPos, _T("m_clrCurLinNumber"), m_clrCurLinNumber);  
	app->WriteProfileInt( strColorPos, _T("m_clrLinNumberSep"), m_clrLinNumberSep);  
	app->WriteProfileInt( strColorPos, _T("m_clrCross"), m_clrCross);  
	app->WriteProfileInt( strColorPos, _T("m_clrTab"), m_clrTab);  
	app->WriteProfileInt( strColorPos, _T("m_clrBKSelText"), m_clrBKSelText);  

	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfHeight"), m_lf.lfHeight);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfWidth"), m_lf.lfWidth);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfEscapement"), m_lf.lfEscapement);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfOrientation"), m_lf.lfOrientation);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfWeight"), m_lf.lfWeight);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfItalic"), m_lf.lfItalic);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfUnderline"), m_lf.lfUnderline);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfStrikeOut"), m_lf.lfStrikeOut);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfCharSet"), m_lf.lfCharSet);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfOutPrecision"), m_lf.lfOutPrecision);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfClipPrecision"), m_lf.lfClipPrecision);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfItalic"), m_lf.lfItalic);  
	app->WriteProfileInt(_T("icrEdit_Font"), _T("lfQuality"), m_lf.lfQuality);  
	app->WriteProfileString(_T("icrEdit_Font"), _T("lfFaceName"), m_lf.lfFaceName);  
}

void CSynEditView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	InvalidateRect(m_rcClient, FALSE);
	m_bAllowDraw = FALSE;
	CRichEditView::OnHScroll(nSBCode, nPos, pScrollBar);
	m_bAllowDraw = TRUE;
	ValidateRect(m_rcClient);

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	GetScrollInfo(SB_HORZ, &si);
	if(nSBCode==SB_THUMBPOSITION || nSBCode==SB_THUMBTRACK)
		m_bTrack = TRUE;
	else
		m_bTrack = FALSE;
	SendMessage(WM_PAINT, 0, 0);
}

// 设置m_ptEnd 和 m_ptStart 的值
void CSynEditView::GetSelection()
{
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();	
	CHARRANGE cr;
	SynCtrl.GetSel( cr ); 
	m_ptStart = SynCtrl.GetCharPos( cr.cpMin ); 
	m_ptEnd = SynCtrl.GetCharPos( cr.cpMax );
}

BOOL CSynEditView::IsStrInSelection(int ptLeft, int ptTop, BOOL bleft)
{
	if( m_ptStart == m_ptEnd ) //如果没有选定直接返回FALSE

		return FALSE;

	int nhalfcharwidth = m_nCharSpaceWidth/2;
	if(bleft) //如果判断当前点是否在选定区域的左边，则ptLeft应加nhalfcharwidth
		ptLeft+=nhalfcharwidth;
	else
		ptLeft-=nhalfcharwidth; //否则减nhalfcharwidth

	if( m_ptStart.y == m_ptEnd.y ) { //如果选定行为单行
		if( ptTop != m_ptStart.y ) //如果光标不在该行
			return FALSE;
		else {
			if( ptLeft >= m_ptStart.x && ptLeft <= m_ptEnd.x)
				return TRUE;
			else
				return FALSE;
		}
	}
	else {
		if( ptTop == m_ptStart.y ) { //如果当前位置在选取定行的第一行
			if( ptLeft <  m_ptStart.x )
				return FALSE;
			else
				return TRUE;
		}
		if( ptTop == m_ptEnd.y ) { //如果当前位置在选定行的最后一行
			if( ptLeft < m_ptEnd.x )
				return TRUE;
			else
				return FALSE;
		}
		else if( ptTop > m_ptStart.y && ptTop < m_ptEnd.y ) //如果当前位置在选定行的中间
			return TRUE;
		else
			return FALSE;
	}
}

void CSynEditView::JudgeInSeletioAndDrawText(CDC *cacheDC, CRect &rcLine, CString &str, COLORREF clrBkColor, COLORREF clrText)
{
//*
	COLORREF clrSelText;

	int nFormat = DT_LEFT|DT_NOPREFIX|DT_EXPANDTABS|DT_VCENTER|DT_SINGLELINE|DT_RTLREADING;	
	CSize size = cacheDC->GetTextExtent(str); 
	int nOffsetY = (rcLine.Height()-size.cy)/2;
	if( !IsSelectionInStr(rcLine.left, size.cx, rcLine.top) ) {
		BOOL bStrLeft = IsStrInSelection( rcLine.left, rcLine.top, TRUE );
		BOOL bStrRight = IsStrInSelection( rcLine.left + size.cx, rcLine.top, FALSE );
		
		if( !bStrLeft && !bStrRight )  //如果str的左右都不在选定区域内
		{
		/*	if(rcLine.top == m_ptEnd.y && rcLine.left == m_ptEnd.x)
			{
				//这段代码是画最后一行的选定区域标志
				CRichEditCtrl &SynCtrl = GetRichEditCtrl();	
				int nLineCount = SynCtrl.GetLineCount();
				int nOffset = SynCtrl.LineIndex( nLineCount-1 );
				int nLineLength = SynCtrl.LineLength(nOffset);
				CPoint pt = SynCtrl.GetCharPos( nOffset+nLineLength );
				if(0 == nLineLength && pt == m_ptEnd)
				{
					CRect rect(rcLine);
					rect.right = rect.left + size.cx;
					cacheDC->FillSolidRect( &rect, m_clrBKSelText ); 
					rcLine.OffsetRect( size.cx, 0 );
					return;
				}
			}
			else
			{*/
				cacheDC->SetBkColor( clrBkColor ); 
				cacheDC->SetTextColor( clrText );
				cacheDC->TextOut( rcLine.left, rcLine.top+nOffsetY, str);
				rcLine.OffsetRect( size.cx, 0 );
				return;
			//}
		}
		else if( bStrLeft && bStrRight ) { //如果str的左右都在选定区域内
			CRect rect(rcLine);
			rect.right = rect.left + size.cx;
			cacheDC->FillSolidRect( &rect, m_clrBKSelText ); 
			clrSelText = RGB( 255-GetRValue(clrText), 255-GetGValue(clrText), 255-GetBValue(clrText) );
			cacheDC->SetTextColor( clrSelText );
			cacheDC->TextOut( rcLine.left, rcLine.top+nOffsetY, str);
			rcLine.OffsetRect( size.cx, 0 );
			return;
		}
	}
	//如果str部分在选定区域内, 或者选定区域在str内执行以下代码
	CString strChar;
	for( int nChar = 0; nChar < str.GetLength(); nChar++ ) {
		if(IsDBCSLeadByte(str[nChar]))
		{
			//如果是双字节字符如中文，则取该中文
			strChar = str.Mid( nChar, 2 );
			nChar++;
		}
		else
		{
			strChar = str.Mid( nChar, 1 );
		}

		size = cacheDC->GetTextExtent( strChar ); 
		BOOL bStrLeft = IsStrInSelection( rcLine.left, rcLine.top, TRUE );
		BOOL bStrRight = IsStrInSelection( rcLine.left + size.cx, rcLine.top, FALSE );
		if( bStrLeft && bStrRight ) { 
			CRect rect(rcLine);
			rect.right = rect.left + size.cx;
			cacheDC->FillSolidRect( &rect, m_clrBKSelText ); 
			clrSelText = RGB( 255-GetRValue(clrText), 255-GetGValue(clrText), 255-GetBValue(clrText) );
			cacheDC->SetTextColor( clrSelText );
		}
		else {
			cacheDC->SetBkColor( clrBkColor ); 
			cacheDC->SetTextColor( clrText );
		}
		cacheDC->TextOut( rcLine.left, rcLine.top+nOffsetY, strChar);
		rcLine.OffsetRect( size.cx, 0 );
	}
//*/	
/*
	COLORREF clrSelText;

	int nFormat = DT_LEFT|DT_NOPREFIX|DT_EXPANDTABS|DT_VCENTER|DT_SINGLELINE;
	CSize size = cacheDC->GetTextExtent(str); 
	if( !IsSelectionInStr(rcLine.left, size.cx, rcLine.top) ) {
		//如果整个选定区域不在str内，则可能情况是str不在选定区域内、str在选定区域内、str部分在选定区域内
		BOOL bStrLeft = IsStrInSelection( rcLine.left, rcLine.top, TRUE );
		BOOL bStrRight = IsStrInSelection( rcLine.left + size.cx, rcLine.top, FALSE );
		
		if( !bStrLeft && !bStrRight ) { //如果str的左右都不在选定区域内			
			cacheDC->SetBkColor( clrBkColor ); 
			cacheDC->SetTextColor( clrText );
			cacheDC->TextOut( rcLine.left, rcLine.top, str);
			rcLine.OffsetRect( size.cx, 0 );
			return;
		}
		else if( bStrLeft && bStrRight ) { //如果str的左右都在选定区域内
			CRect rect(rcLine);
			rect.right = rect.left + size.cx;
			cacheDC->FillSolidRect( &rect, m_clrBKSelText ); 
			clrSelText = RGB( 255-GetRValue(clrText), 255-GetGValue(clrText), 255-GetBValue(clrText) );
			cacheDC->SetTextColor( clrSelText );
			cacheDC->TextOut( rcLine.left, rcLine.top, str);
			rcLine.OffsetRect( size.cx, 0 );
			return;
		}
	}
//如果选定区域在str中或str部分在选定区域中则往下做
//方法：将str按选定分为在块，中间一块为选定的文本，两边为没有选定的文本
	if(str.IsEmpty())
		return;

	size = (0, 0);
	CString strtmp;
	int nleft, nright;
	for( nleft = 0; nleft <= str.GetLength(); nleft++ ) {
		if( rcLine.left + size.cx >= m_ptStart.x )
			break;
		strtmp = str.Left( nleft ); 
		size = cacheDC->GetTextExtent( strtmp ); 
	}	
	if(--nleft<0)
		nleft = 0;

	size = (0, 0);
	for( nright = 0; nright <= str.GetLength(); nright++ ) {
		if( rcLine.left + size.cx >= m_ptEnd.x )
			break;
		strtmp = str.Left( nright ); 
		size = cacheDC->GetTextExtent( strtmp ); 
	}
	--nright;

	//取最左的一块
	strtmp = str.Left( nleft );
	if(!strtmp.IsEmpty()) {
		CRect rect(rcLine);
		size = cacheDC->GetTextExtent( strtmp );
		rect.right = rect.left + size.cx;
		cacheDC->FillSolidRect( &rect, clrBkColor ); 
		cacheDC->SetTextColor( clrText );
		cacheDC->TextOut( rcLine.left, rcLine.top, str);
		rcLine.OffsetRect( size.cx, 0 );
	}

	//取中间的一块
	strtmp = str.Mid( nleft, nright - nleft );
	if(!strtmp.IsEmpty()) {
		CRect rect(rcLine);
		size = cacheDC->GetTextExtent( strtmp );
		rect.right = rect.left + size.cx;
		clrSelText = RGB( 255-GetRValue(clrText), 255-GetGValue(clrText), 255-GetBValue(clrText) );
		cacheDC->FillSolidRect( &rect, m_clrBKSelText ); 
		cacheDC->SetTextColor( clrSelText );
		cacheDC->TextOut( rcLine.left, rcLine.top, str);
		rcLine.OffsetRect( size.cx, 0 );
	}

	//取最右的一块
	strtmp = str.Mid( nright, str.GetLength() - nright );
	if(!strtmp.IsEmpty()) {
		CRect rect(rcLine);
		size = cacheDC->GetTextExtent( strtmp );
		rect.right = rect.left + size.cx;
		cacheDC->FillSolidRect( &rect, clrBkColor ); 
		cacheDC->SetTextColor( clrText );
		cacheDC->TextOut( rcLine.left, rcLine.top, str);
		rcLine.OffsetRect( size.cx, 0 );
	}
//*/
}

//计算水平滚动位置，以便在水平滚动时可以正确显示
void CSynEditView::CalcHorzScrollPos()
{
	if(m_nWrapMode == WrapToWindow) 
	{
		m_nHorzPos = 0;
		return;
	}

	SCROLLINFO si;
	
	si.cbSize = sizeof(si);
	GetScrollInfo(SB_HORZ, &si);
	
	if( m_bTrack ) 
		m_nHorzPos = si.nTrackPos;
	else
		m_nHorzPos = si.nPos;
}


void CSynEditView::DrawSynEditView( int nShowLine )
{
	if(!m_bAllowDraw)
 		return;

  	CRichEditCtrl &SynCtrl = GetRichEditCtrl();
	m_nLineCount = SynCtrl.GetLineCount(); 
 	m_nCurrentLine = GetCurrentLine();

	int i = 0;
	int nLineCount = m_nLineCount;
	while(nLineCount!=0)
	{
		i++;
		nLineCount /= 10;
	}
	if(m_nLineNumberCharNumber!=i)
	{
		m_nLineNumberCharNumber = i;
		SetLeftMargin();
		PostMessage(WM_PAINT);
	}

 	if (m_pdwParseCookies != NULL)
 	{
 		if (m_nParseArraySize != m_nLineCount)
 		{
			int nCurrentLine = m_nCurrentLine;
			if(m_nParseArraySize < m_nLineCount) {
				nCurrentLine = m_nParseArraySize + m_nCurrentLine - m_nLineCount; 
				if(nCurrentLine<0)
					nCurrentLine = m_nCurrentLine;
			}
			if(m_nCurrentLine>m_nLineCount)
				nCurrentLine=0;
 			//	Reallocate cookies array
 			DWORD *pdwNewArray = new DWORD[m_nLineCount];
 			if (m_nCurrentLine > 0)
 				memcpy(pdwNewArray, m_pdwParseCookies, sizeof(DWORD) * nCurrentLine);
 			delete m_pdwParseCookies;
 			m_nParseArraySize = m_nLineCount;
 			m_pdwParseCookies = pdwNewArray;
			memset(m_pdwParseCookies + nCurrentLine, 0xff, sizeof(DWORD) * (m_nParseArraySize - nCurrentLine));
 		}
 	}
 	
 	CDC *pdc=GetDC();
 	CDC cacheDC;
 	VERIFY(cacheDC.CreateCompatibleDC(pdc));
 	if (m_pCacheBitmap == NULL)
 	{
 		m_pCacheBitmap = new CBitmap;
 		m_pCacheBitmap->CreateCompatibleBitmap(pdc, m_rcClient.Width(), m_rcClient.Height());  //nLineHeight);
 	}
 	CBitmap *pOldBitmap = cacheDC.SelectObject(m_pCacheBitmap);
 
	GetSelection();
	CalcHorzScrollPos();

	CString str;
	int nLine = SynCtrl.GetFirstVisibleLine();
 	if(nLine==0)
		m_bRealReturn = TRUE;
	else
		m_bRealReturn = GetLineString(nLine-1, str);

	//将文本显示区域背景色画好
	CRect rcRightText(m_rcClient);
	rcRightText.left = m_nLeftMargin;
	cacheDC.FillSolidRect(&rcRightText, m_clrBkColor); 	
	rcRightText.left = m_nLeftMargin - m_nHorzPos;

	int nLinesTop = GetLinesTop(nLine); //第一行要计算好偏移值，因为第一行可能只显示半行出来
	int nLinesTopTemp = nLinesTop;

	//*
	rcRightText.top = nLinesTop;
	rcRightText.bottom = nLinesTop + m_nLineHeight;

	//calc the number ready to draw
	int nNumberToDraw = 0;
	while(nLinesTop <= m_rcClient.bottom ) {
		if(nLine + nNumberToDraw > m_nLineCount-1)
			break;
		nLinesTop += m_nLineHeight;
		++nNumberToDraw;
	}

	for(i=0; i<nNumberToDraw; i++) {
		DrawSingleLineColorText(&cacheDC, nLine+i, rcRightText);
		rcRightText.OffsetRect(0, m_nLineHeight );
	}

  	//将行号显示区域背景色画好
	CRect rcLeftLineNumber(m_rcClient);
	CRect rcLineNumberRight(m_rcClient);
	if(m_bShowLineNumber) {
		rcLeftLineNumber.right =  m_nLeftMargin;
		cacheDC.FillSolidRect(&rcLeftLineNumber, m_clrBkColor); 
		CPen NewPen(PS_SOLID, 5, m_clrLinNumberSep);   // 画行号栏
		CPen *OldPen = cacheDC.SelectObject(&NewPen); 
		cacheDC.MoveTo(m_nLeftMargin - m_nDefaultLeftMargin, rcLeftLineNumber.top);
		cacheDC.LineTo(m_nLeftMargin - m_nDefaultLeftMargin, rcLeftLineNumber.bottom);
		cacheDC.SelectObject(OldPen); 
		
		rcLeftLineNumber.right =  m_nLeftMargin - m_nDefaultLeftMargin;
		cacheDC.FillSolidRect(&rcLeftLineNumber, m_clrLinNumberBkColor); 
	}
	else {
		rcLeftLineNumber.right = m_nDefaultLeftMargin;
		cacheDC.FillSolidRect(&rcLeftLineNumber, m_clrBkColor); 
	}

	rcLeftLineNumber.top = nLinesTopTemp;
	rcLeftLineNumber.bottom = nLinesTopTemp + m_nLineHeight;
	rcLeftLineNumber.right -= 2;

	for(i=0; i<nNumberToDraw; i++) 
	{
		DrawLineNumber( &cacheDC, nLine+i, rcLeftLineNumber );
		rcLeftLineNumber.OffsetRect(0, m_nLineHeight);
	}

	if(m_bShowCross)
		DrawCross(&cacheDC);

//////////////////////////
	if ( nShowLine != -1 )
	{
		//CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)) ;
		//CBrush *pOldBrush=cacheDC.SelectObject(pBrush);//选入设备描述表
		//CPen *pPen = new CPen( PS_SOLID, 1, RGB(255,0,0) );
		//CPen *pOldPen = cacheDC.SelectObject( pPen );
		//showLineCode( nShowLine, &cacheDC );
		//cacheDC.SelectObject( pOldPen );
		//cacheDC.SelectObject(pOldBrush);
	}

///////////////////////////////	


	pdc->BitBlt(m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(), &cacheDC, 0, 0, SRCCOPY);
	
	cacheDC.SelectObject(pOldBitmap);
 	cacheDC.DeleteDC();	

	ReleaseDC(pdc);
}

void CSynEditView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();	
	m_bAllowDraw = FALSE;

	CHARRANGE cr;
	SynCtrl.GetSel( cr ); 

	CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);

	if(cr.cpMax != cr.cpMin)
	{
		if(VK_DELETE==nChar || VK_BACK==nChar)
		{
			ReCalcCursorPostion();
		}
	}
/*
//放弃以下功能是因为ReCalcCursorPostion这个函数
	if(nChar==VK_RETURN)
	{
		int nline = GetCurrentLine();
		if(nline!=0)
		{
			CString strtmp, strline;
			GetLineString(--nline, strline);
			strtmp = strline;
			strtmp.TrimLeft(0x20);
			strtmp.TrimLeft(0x9);
			int npos = strline.Find(strtmp);
			if(npos!=0) 
			{
				strtmp = strline.Left(npos);
				SynCtrl.ReplaceSel(strtmp, TRUE); 
			}
		}
	}
//*/
	m_bAllowDraw = TRUE;

	GetSelection();
	if(nChar == VK_LEFT || nChar == VK_RIGHT || nChar == VK_UP || nChar == VK_DOWN) 
	{
		SendMessage(WM_PAINT, 0, 0);
	}

	SynCtrl.GetSel( cr ); 
	if(cr.cpMin != cr.cpMax) 
	{
		SendMessage(WM_PAINT, 0, 0);
	}
	else if (m_nCurrentLine != GetCurrentLine()) 
	{
		SendMessage(WM_PAINT, 0, 0);
	}

	int nHorzPos = m_nHorzPos;
	CalcHorzScrollPos();
	if(nHorzPos != m_nHorzPos)
	{
		SendMessage(WM_PAINT, 0, 0);
	}
}

void CSynEditView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRichEditView::OnLButtonDown(nFlags, point);

	//showLineCode( 100 );


	int nCurLine;
	BOOL bFlag;
	CRichEditCtrl &SynCtrl=GetRichEditCtrl();

	if(m_bShowLineNumber)
		m_nLeftMargin = m_nLineNumberCharNumber * m_nCharNumberWidth + m_nDefaultLeftMargin + 4 ;
	else
		m_nLeftMargin =  m_nDefaultLeftMargin;

	if (!(point.x <= m_nLeftMargin)) // 在特定区域才可以设置断点
	{
		SendMessage(WM_PAINT, 0, 0);
		return;
	}
	
	nCurLine = point.y/m_nLineHeight;
	nCurLine += SynCtrl.GetFirstVisibleLine();
	bFlag = TRUE;
    for ( std::list< int >::iterator iter = breakPoint.begin(); iter != breakPoint.end(); iter++ )
	{
		if ( *iter == nCurLine) 
		{
			removeBreakPoint( nCurLine );
			bFlag = FALSE;
			break;
		}
	}
	if ( bFlag )
	{
		setBreakPoint( nCurLine );
	}

	SendMessage(WM_PAINT, 0, 0);
	
	
}

void CSynEditView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CRichEditView::OnKeyUp(nChar, nRepCnt, nFlags);
	SendMessage(WM_PAINT, 0, 0);
}


BOOL CSynEditView::IsSelectionInStr(int nFromHere, int nStrWidth, int nTop)
{
	if( m_ptStart == m_ptEnd )
		return FALSE;

	if( m_ptStart.y != m_ptEnd.y )  //如果选定行不为单行，返回，因为字符不可能跨行
		return FALSE;

	if( m_ptStart.y != nTop )  //如果待测试行的顶部不等于选定行的顶部则返回
		return FALSE;

	if( (m_ptStart.x > nFromHere) && (m_ptEnd.x < nFromHere + nStrWidth) )
		return TRUE;
	else
		return FALSE;
}

int CSynEditView::GetLanguage()
{
	return m_nLanguage;
}


void CSynEditView::SetColor(int nColorIndex, COLORREF clrNew)
{
	switch(nColorIndex) {
	case COLORINDEX_BK:
		m_clrBkColor = clrNew;
		break;
	case COLORINDEX_COMMENT:
		m_clrCommentColor = clrNew;
		break;
	case COLORINDEX_SYNTAX:
		m_clrSyntaxColor = clrNew;
		break;
	case COLORINDEX_NORMAL:
		m_clrNormalColor = clrNew;
		break;
	case COLORINDEX_STRING:
		m_clrStringColor = clrNew;
		break;
	case COLORINDEX_CHAR:
		m_clrCharColor = clrNew;
		break;
	case COLORINDEX_NUMBER:
		m_clrNumberColor = clrNew;
		break;
	case COLORINDEX_UNDERLINE:
		m_clrUnderLine = clrNew;
		break;
	case COLORINDEX_LINEEND:
		m_clrLineEnd = clrNew;
		break;
	case COLORINDEX_BKCURLINE:
		m_clrBkCurLine = clrNew;
		break;
	case COLORINDEX_LINNUMBERBKCOLOR:
		m_clrLinNumberBkColor = clrNew;
		break;
	case COLORINDEX_LINNUMBERNORMAL:
		m_clrLinNumberNormal = clrNew;
		break;
	case COLORINDEX_CURLINNUMBER:
		m_clrCurLinNumber = clrNew;
		break;
	case COLORINDEX_LINNUMBERSEP:
		m_clrLinNumberSep = clrNew;
		break;
	case COLORINDEX_CROSS:
		m_clrCross = clrNew;
		break;
	case COLORINDEX_TAB:
		m_clrTab = clrNew;
		break;
	case COLORINDEX_BKSELTEXT:
		m_clrBKSelText = clrNew;
		break;
	}
	WriteSettings();
}

COLORREF CSynEditView::GetColor(int nColorIndex)
{
	switch(nColorIndex) {
	case COLORINDEX_BK:
		return m_clrBkColor;
	case COLORINDEX_COMMENT:
		return m_clrCommentColor;
	case COLORINDEX_SYNTAX:
		return m_clrSyntaxColor;
	case COLORINDEX_NORMAL:
		return m_clrNormalColor;
	case COLORINDEX_STRING:
		return m_clrStringColor;
	case COLORINDEX_CHAR:
		return m_clrCharColor;
	case COLORINDEX_NUMBER:
		return m_clrNumberColor;
	case COLORINDEX_UNDERLINE:
		return m_clrUnderLine;
	case COLORINDEX_LINEEND:
		return m_clrLineEnd;
	case COLORINDEX_BKCURLINE:
		return m_clrBkCurLine;
	case COLORINDEX_LINNUMBERBKCOLOR:
		return m_clrLinNumberBkColor;
	case COLORINDEX_LINNUMBERNORMAL:
		return m_clrLinNumberNormal;
	case COLORINDEX_CURLINNUMBER:
		return m_clrCurLinNumber;
	case COLORINDEX_LINNUMBERSEP:
		return m_clrLinNumberSep;
	case COLORINDEX_CROSS:
		return m_clrCross;
	case COLORINDEX_TAB:
		return m_clrTab;
	case COLORINDEX_BKSELTEXT:
		return m_clrBKSelText;
	default:
		return m_clrNormalColor;
	}
}

void CSynEditView::ReadColorInfo(int nLanguage)
{
	CString strColorPos = _T("icrEdit_Color");
	strColorPos += GetLanguageString(nLanguage);

	CWinApp *app = AfxGetApp();
	m_clrBkColor = app->GetProfileInt( strColorPos, _T("m_clrBkColor"), RGB(255, 255, 255));  
	m_clrCommentColor = app->GetProfileInt( strColorPos, _T("m_clrCommentColor"), RGB(0, 128, 0));  
	m_clrSyntaxColor = app->GetProfileInt( strColorPos, _T("m_clrSyntaxColor"), RGB(0, 0, 255));  
	m_clrNormalColor = app->GetProfileInt( strColorPos, _T("m_clrNormalColor"), RGB(0, 0, 0));  
	m_clrStringColor = app->GetProfileInt( strColorPos, _T("m_clrStringColor"), RGB(128, 0, 0));  
	m_clrCharColor = app->GetProfileInt( strColorPos, _T("m_clrCharColor"), RGB(128, 0, 0));  
	m_clrNumberColor = app->GetProfileInt( strColorPos, _T("m_clrNumberColor"), RGB(255, 0, 0));  
	m_clrUnderLine = app->GetProfileInt( strColorPos, _T("m_clrUnderLine"), RGB(200, 200, 200));  
	m_clrLineEnd = app->GetProfileInt( strColorPos, _T("m_clrLineEnd"), RGB(0, 0, 200));  
	m_clrBkCurLine = app->GetProfileInt( strColorPos, _T("m_clrBkCurLine"), RGB(200, 200, 200));  

	m_clrLinNumberBkColor = app->GetProfileInt( strColorPos, _T("m_clrLinNumberBkColor"), RGB(255, 255, 255));  
	m_clrLinNumberNormal = app->GetProfileInt( strColorPos, _T("m_clrLinNumberNormal"), RGB(192, 192, 192));  
	m_clrCurLinNumber = app->GetProfileInt( strColorPos, _T("m_clrCurLinNumber"), RGB(0, 0, 128));  
	m_clrLinNumberSep = app->GetProfileInt( strColorPos, _T("m_clrLinNumberSep"), RGB(192, 192, 192));  
	m_clrCross = app->GetProfileInt( strColorPos, _T("m_clrCross"), RGB(0, 0, 128));  
	m_clrTab = app->GetProfileInt( strColorPos, _T("m_clrTab"), RGB(128, 128, 128));  
	m_clrBKSelText = app->GetProfileInt( strColorPos, _T("m_clrBKSelText"), RGB(0, 0, 128));  	
}


BOOL CSynEditView::IsShowLineNumber()
{
	return m_bShowLineNumber;
}

BOOL CSynEditView::IsShowSelectLine()
{
	return m_bShowSelectLine;
}

BOOL CSynEditView::IsShowUnderLine()
{
	return m_bShowUnderLine;
}

BOOL CSynEditView::IsShowReturnLineToken()
{
	return m_bShowReturnLineToken;
}

int CSynEditView::GetTabSize()
{
	return m_nTabSize;
}


CString CSynEditView::GetSingleCommentString(int nlanguage)
{
	return GetLineCommentString(nlanguage);
}

void CSynEditView::DrawReturnLineToken(BOOL bRealReturn, CDC *cacheDC, CRect &rcline)
{
	rcline.OffsetRect(-m_nCharSpaceWidth-2, 0);
	int nlineheight = rcline.Height(); 
	int nleft = rcline.left;
	int ntop = rcline.top + nlineheight/4 + 1 ;
	int nbottom = rcline.bottom - nlineheight/4;
	if(bRealReturn) {
		for(int i=ntop; i<nbottom; i++)
			cacheDC->SetPixel( nleft + 5, i+1, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 2, nbottom - 3, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 3, nbottom - 2, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 4, nbottom - 1, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 6, nbottom - 1, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 7, nbottom - 2, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 8, nbottom - 3, m_clrLineEnd );
	}
	else {
		int nhalfline = rcline.top + nlineheight/2;
		for(int i=nleft+2; i<nleft+8; i++)
			cacheDC->SetPixel( i, nhalfline, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 7, nhalfline-1, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 7, nhalfline+1, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 6, nhalfline-2, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 6, nhalfline+2, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 5, nhalfline-3, m_clrLineEnd );
		cacheDC->SetPixel( nleft + 5, nhalfline+3, m_clrLineEnd );
	}
}



void CSynEditView::GetSelLine(int &nfirstline, int &nendline)
{
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();

	CHARRANGE cr;
	SynCtrl.GetSel( cr );

	//计算选定区域的第一行和最后一行
	nfirstline = SynCtrl.LineFromChar(cr.cpMin); 
	nendline = SynCtrl.LineFromChar(cr.cpMax);
	if(SynCtrl.LineFromChar(cr.cpMax-1)<nendline)
		--nendline;
	if(nendline<nfirstline)
		nendline = nfirstline; 
	//////////////////////////////////
}


BOOL CSynEditView::IsReadOnly()
{
	return m_bReadOnly;
}

void CSynEditView::SetReadOnly(BOOL bReadOnly)
{
	m_bReadOnly = bReadOnly;
	GetRichEditCtrl().SetReadOnly(m_bReadOnly); 
	WriteSettings();
}

//这个函数的作用是在粘贴时重置行nParseArraySize的语法解析缓冲区, 调用位置在CMainFrame的粘贴处理函数
void CSynEditView::SetParseCookieZeroFromGivenLine(int nParseArraySize)
{
	memset(m_pdwParseCookies + nParseArraySize, 0xff, sizeof(DWORD) * (m_nParseArraySize - nParseArraySize));
}

int CSynEditView::GetUndoLimits()
{
	return m_nUndoLimits;
}

void CSynEditView::SetUndoLimits(int nMax)
{
	m_nUndoLimits = nMax;
	SendMessage(EM_SETUNDOLIMIT, m_nUndoLimits, 0);
	WriteSettings();
}


BOOL CSynEditView::IsAutoWordSelect()
{
//	const DWORD eco = SendMessage(EM_GETOPTIONS, 0, 0);
	LRESULT eco = SendMessage(EM_GETOPTIONS, 0, 0);
	if(eco & ECO_AUTOWORDSELECTION)
		return TRUE;
	else 
		return FALSE;
}

void CSynEditView::SetAutoWordSelect(BOOL bAutoSelect)
{
	if(IsAutoWordSelect()) {
		if(!bAutoSelect)
			SendMessage(EM_SETOPTIONS, ECOOP_XOR, ECO_AUTOWORDSELECTION);
	}
	else {
		if(bAutoSelect)
			SendMessage(EM_SETOPTIONS, ECOOP_XOR, ECO_AUTOWORDSELECTION);
	}
}



void CSynEditView::DrawCross(CDC *cacheDC)
{
	//if(m_ptStart!=m_ptEnd)
	//	return;
	//
	//CPen NewPen(PS_SOLID, 1, m_clrCross);
	//CPen *OldPen = cacheDC->SelectObject(&NewPen); 
	//cacheDC->MoveTo(m_nLeftMargin, m_ptStart.y+m_nLineHeight);
	//cacheDC->LineTo(m_rcClient.Width(), m_ptStart.y+m_nLineHeight);
	//cacheDC->MoveTo(m_ptStart.x, 0);
	//cacheDC->LineTo(m_ptStart.x, m_rcClient.Height());
	//cacheDC->SelectObject(OldPen); 
}

void CSynEditView::ShowCross(BOOL bShow)
{
	m_bShowCross = bShow;
}

BOOL CSynEditView::IsShowCross()
{
	return m_bShowCross;
}

void CSynEditView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	InvalidateRect(m_rcClient, FALSE);
	m_bAllowDraw = FALSE;
	CRichEditView::OnVScroll(nSBCode, nPos, pScrollBar);
	m_bAllowDraw = TRUE;
	ValidateRect(m_rcClient);

	SendMessage(WM_PAINT, 0, 0);
}

BOOL CSynEditView::IsShowTabSpace()
{
	return m_bShowTab;
}

void CSynEditView::ShowTabSpace(BOOL bShow)
{
	m_bShowTab = bShow;
}

int CSynEditView::GetVisibleLineCount()
{
	return m_rcClient.Height() / m_nLineHeight;
}

int CSynEditView::GetSpaceWidth()
{
	return m_nCharSpaceWidth;
}

void CSynEditView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawSynEditView( );	
	// Do not call CRichEditView::OnPaint() for painting messages
}

void CSynEditView::InitEditorFont()
{
	SetWindowText(""); //此行不必要，因为初始化时内容必为空，同时会引导致启动程序时内容为空
	SetSynEditViewFont(m_lf); //不会改文本内容

	//将tab宽设置好
	SetSynCtrlTabSize(m_nTabSize);  //不会改文本内容
	/////////////////

	//EM_SETTEXTMODE必须要内容为空时才有效
	SendMessage(EM_SETTEXTMODE, TM_PLAINTEXT|TM_MULTILEVELUNDO|TM_SINGLECODEPAGE, 0);
}

void CSynEditView::OnSetFocus(CWnd* pOldWnd) 
{
	CRichEditView::OnSetFocus(pOldWnd);
	SendMessage(WM_PAINT, 0, 0);
}

//从RichEditCtrl中流出文本
//文本流入到RichEditCtrl中
//dwCookie	-	送入的一段文本的首地址，文本长度通常为2k
//pbBuff	-	可以理解为RichEditCtrl的文本缓冲区
//cb		-	送来的文本长度，通常为2k
//pcb		-	送入的文本长度，将会回调给RichEditCtrl
DWORD CALLBACK EditStreamCallbackOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
/*
	//下面的代码来自MFC Programmer's SourceBook
	As noted in Article ID: Q136810 of Microsoft Knowlage Base: The EditStreamCallback 
	function is called until *pcb receives a zero value.
	I found this problem only appear when work with win9x (in win2k EditStreamCallback 
	never called again for normal text if *pcb gets value less than cb)
	So you can use next techniques for stream in (& out ) normal text:
*/
	CString* strWholeString = reinterpret_cast<CString*>(dwCookie);
	
	// I hope this allocation/deallocation better then manipulation with CStrings
	char* szInThisCall = new char[cb+1];
	memcpy( szInThisCall, reinterpret_cast<char*>(pbBuff), cb );
	szInThisCall[cb] = '\0';

	// This is the main trik! Control can call this function for just a part of string!
	(*strWholeString ) += szInThisCall;
	
	delete	[] szInThisCall;

	*pcb = cb;
 	return 0;
}

void CSynEditView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();	
	CHARRANGE cr;
	SynCtrl.GetSel(cr);
	const int nSpecialChar = 5;
	static int nHasSpecialChar[ nSpecialChar ];               // } ) ] ' "

	if ( (char)nChar == '}' && nHasSpecialChar[0] == TRUE )
	{
		cr.cpMax += 1;
		cr.cpMin += 1;
		SynCtrl.SetSel(cr);
		nHasSpecialChar[0] = FALSE;
		return;
	}
	if ( (char)nChar == ')' && nHasSpecialChar[1] == TRUE )
	{
		cr.cpMax += 1;
		cr.cpMin += 1;
		SynCtrl.SetSel(cr);
		nHasSpecialChar[1] = FALSE;
		return;
	}
	if ( (char)nChar == ']' && nHasSpecialChar[2] == TRUE )
	{
		cr.cpMax += 1;
		cr.cpMin += 1;
		SynCtrl.SetSel(cr);
		nHasSpecialChar[2] = FALSE;
		return;
	}
	if ( (char)nChar == '\'' && nHasSpecialChar[3] == 2 )
	{
		cr.cpMax -= 1;
		cr.cpMin -= 1;
		SynCtrl.SetSel(cr);
		nHasSpecialChar[3] = 3;
		return;
	}
	else if ( (char)nChar == '\'' && nHasSpecialChar[3] == 3 )
	{
		cr.cpMax += 1;
		cr.cpMin += 1;
		SynCtrl.SetSel(cr);
		nHasSpecialChar[3] = 0;
		return;
	}

	if ( (char)nChar == '\"' && nHasSpecialChar[4] == 2 )
	{
		cr.cpMax -= 1;
		cr.cpMin -= 1;
		SynCtrl.SetSel(cr);
		nHasSpecialChar[4] = 3;
		return;
	}
	else if ( (char)nChar == '\"' && nHasSpecialChar[4] == 3 )
	{
		cr.cpMax += 1;
		cr.cpMin += 1;
		SynCtrl.SetSel(cr);
		nHasSpecialChar[4] = 0;
		return;
	}
	CRichEditView::OnChar(nChar, nRepCnt, nFlags);
	
	if(cr.cpMax != cr.cpMin)
	{
		ReCalcCursorPostion();
	}
	else
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		GetScrollInfo(SB_HORZ, &si);
		m_nHorzMaxOld = si.nMax;
	}
	formatCode( nChar, cr, nHasSpecialChar );
}


void CSynEditView::ReCalcCursorPostion()
{
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();	

	SCROLLINFO si;	
	si.cbSize = sizeof(si);
	GetScrollInfo(SB_HORZ, &si);

	if(si.nMax != m_nHorzMaxOld)
	{
		m_nHorzMaxOld = si.nMax;
		SendMessage(WM_KEYDOWN, 0xd, 0);
		SendMessage(WM_KEYDOWN, VK_BACK, 0);

		if(GetCurrentLine()!=0)
		{
			SendMessage(WM_KEYDOWN, VK_UP, 0);
			SendMessage(WM_KEYDOWN, VK_DOWN, 0);
		}

		if(GetCurrentLine()!=SynCtrl.GetLineCount()-1)
		{
			SendMessage(WM_KEYDOWN, VK_DOWN, 0);
			SendMessage(WM_KEYDOWN, VK_UP, 0);
		}
	}
}

LRESULT CSynEditView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch ( message )
	{
	case WM_SHOWLINE:
		showLineCode( wParam );
		return TRUE;
	case WM_UPDATECONTEXT:
		LoadText( *(CString*)wParam );
		delete (CString*)wParam;
		return TRUE;
	default:
		break;
	}

	CHARRANGE cr;
	if(WM_IME_CHAR == message)
	{
		CRichEditCtrl &SynCtrl = GetRichEditCtrl();	
		SynCtrl.GetSel( cr ); 
	}

	LRESULT lr = CRichEditView::WindowProc(message, wParam, lParam);
	
	if(WM_IME_CHAR == message)
	{
		if(cr.cpMax != cr.cpMin)
		{
			ReCalcCursorPostion();
		}
		else
		{
			SCROLLINFO si;
			si.cbSize = sizeof(si);
			GetScrollInfo(SB_HORZ, &si);
			m_nHorzMaxOld = si.nMax;
		}
	}
	
	return lr;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSynEditView::setBreakPoint( int nLineNumber )
{
	breakPoint.push_back( nLineNumber );
}

void CSynEditView::removeBreakPoint( int nLineNumber )
{
	breakPoint.remove( nLineNumber );
}

void CSynEditView::SelectLine( int nLineNo )
{
	PostMessage( WM_SHOWLINE, nLineNo, 0 );
}

void CSynEditView::showLineCode( int nLineNumber )
{
	CString tempStr;
	CHARRANGE cr;
	CRichEditCtrl &synCtrl = GetRichEditCtrl();
	int nLineMax;
	int firstVisibleline, maxShowLine, nLineCount;;
	nLineMax = synCtrl.GetLineCount();
	if ( nLineMax < nLineNumber )
	{
		nLineNumber = nLineMax;
	}
	if ( nLineNumber < 1 )
	{
		nLineNumber = 1;
	}
	firstVisibleline = synCtrl.GetFirstVisibleLine() + 1;
	nLineCount = GetVisibleLineCount();
	maxShowLine = firstVisibleline + nLineCount;
	if( nLineNumber > maxShowLine )
	{
		synCtrl.LineScroll( (nLineNumber - maxShowLine) + nLineCount/2 );
	}
	if ( nLineNumber < firstVisibleline )
	{
		synCtrl.LineScroll( -(firstVisibleline - nLineNumber)  );
	}
	GotoLine( nLineNumber );
	GetLineString( nLineNumber - 1, tempStr);
	synCtrl.GetSel(cr);
	cr.cpMax += tempStr.GetLength();
	synCtrl.SetSel(cr);

}

void CSynEditView::formatCode( UINT nChar,CHARRANGE cr, int* nHasSpecialChar )
{
	CRichEditCtrl &SynCtrl = GetRichEditCtrl();
	static int sp1, sp2;
	// 成对匹配
	if ( (char)nChar == '{')
	{
		nHasSpecialChar[0] = FALSE;
		PostMessage(WM_CHAR,'}',0);
	}
	if ( (char)nChar == '}')
	{
		SynCtrl.SetSel(cr);
		nHasSpecialChar[0] = TRUE;
	}

	if ( (char)nChar == '(')
	{
		nHasSpecialChar[1] = FALSE;
		this->PostMessage(WM_CHAR,')',0);
	}
	if ( (char)nChar == ')')
	{
		SynCtrl.SetSel(cr);
		nHasSpecialChar[1] = TRUE;
	}


	if ( (char)nChar == '[')
	{
		nHasSpecialChar[2] = FALSE;
		PostMessage(WM_CHAR,']',0);
	}
	if ( (char)nChar == ']')
	{
		SynCtrl.SetSel(cr);
		nHasSpecialChar[2] = TRUE;
	}

	// ' "
	if ( (char)nChar == '\'' && nHasSpecialChar[3] == 0 )
	{
		nHasSpecialChar[3] = 1;
		PostMessage(WM_CHAR,'\'',0);
	}
	else if ( (char)nChar == '\'' && nHasSpecialChar[3] == 1 )
	{
		nHasSpecialChar[3] = 2;
		PostMessage(WM_CHAR,'\'',0);
	}

	if ( (char)nChar == '\"' && nHasSpecialChar[4] == 0 )
	{
		nHasSpecialChar[4] = 1;
		PostMessage(WM_CHAR,'\"',0);
	}
	else if ( (char)nChar == '\"' && nHasSpecialChar[4] == 1 )
	{
		nHasSpecialChar[4] = 2;
		PostMessage(WM_CHAR,'\"',0);
	}

	// 格式设置
	if ( (char)nChar == '\r')
	{
		CString tempStr;
		int countStar , posS, posE, countEnd, count;
		countStar = 0;
		countEnd = 0;
		posS = 0;
		posE = 0;
		SynCtrl.SetSel( 0, cr.cpMax );
		tempStr = SynCtrl.GetSelText();
		// 查找当前位置之前的‘{’和‘}’,并计算个数
		while( posS != -1 || posE != -1 )
		{
			if ( posS != -1)
			{
				posS = tempStr.Find( '{', posS );
			}
			if ( posS != -1 )
			{
				countStar++;
				posS++;
			}
			if ( posE != -1)
			{
				posE = tempStr.Find( '}', posE );
			}

			if ( posE != -1 )
			{
				countEnd++;
				posE++;
			}
			SynCtrl.SetSel( cr.cpMax, cr.cpMax );
		}

		count = countStar - countEnd;
		SynCtrl.SetSel( cr.cpMin, cr.cpMax + 1 );
		tempStr = SynCtrl.GetSelText();
		int pos = tempStr.Find( '}' );
		// 当前位置后一个字符是‘}’则另外处理
		if ( pos == -1 )
		{
			for ( int i = 0; i < count; i++)
			{
				this->PostMessage ( WM_CHAR, '\t', NULL );
			}
		}
		if ( pos != -1 )
		{
			for ( int i = 0; i < count - 1; i++)
			{
				this->PostMessage ( WM_CHAR, '\t', NULL );
			}
		}
		SynCtrl.SetSel( cr.cpMax, cr.cpMax );
	}


}