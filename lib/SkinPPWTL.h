#ifndef _SKINPPWTL_H_
#define _SKINPPWTL_H_

#ifdef _SKINPP_STATIC
	#define SKINPPWTL_API
#else
	#ifdef SKINPPWTL_EXPORTS
		#define SKINPPWTL_API __declspec(dllexport)
	#else
		#define SKINPPWTL_API __declspec(dllimport)
	#endif
#endif

#define WM_TOOLBARPAINTPRE (WM_USER + 802)
#define WM_TOOLBARPAINTEND (WM_USER + 803)

#define SM_LBUTTONUP (WM_USER + 804)

//��ť״̬
enum BUTTONSTATE 
{
	NORMAL	= 0,
	PRESS	= 1,
	DISABLE	= 2,
	HOT		= 3,
	FOCUS   = 4,
	LAST	= 5
};

//��������
enum DRAWTYPE
{
	BUTTON = 0,
	SCROLLARROWUP = 1,
	SCROLLARROWDOWN = 2,
	SPLITTERBARHORZ = 3,
	SPLITTERBARVERT = 4,
	SPLITTERBARBORDER = 5,
	LISTHEAD = 6
};

//���Ƥ����Դ������
enum SKINOBJTYPE
{
	DIALOGTYPE = 0,
	BUTTONTYPE = 1,
	CHECKBOXTYPE = 2,
	RADIOBOXTYPE = 3,
	STATICTYPE = 4,
	TRACKBARTYPE = 5,
};

struct ListBoxItem
{
	HIMAGELIST	hImageList;
	int			nImageIndex;

	ListBoxItem()
	{
		hImageList = NULL;
		nImageIndex = -1;
	}
};

#define REST_BITMAP      0x0001 //.bmp
#define REST_ICON        0x0002 //.ico
#define REST_CURSOR      0x0003 //.cur
#define REST_ANIMATE	 0x0004 //.ani

typedef struct _ResourceInfo
{
	HGDIOBJ hGdiObj;//[OUT]
	DWORD   dwType; //[OUT]
	int		nWidth; //[OUT]
	int		nHeight;//[OUT]

	TCHAR	szResImageName[_MAX_FNAME];//[IN]
	BOOL	bHorzSplit;//[IN]
	int		nLength;//[IN]
	int		nCount; //[IN]
	int		nIndex; //[IN]
	
	_ResourceInfo()
	{
		hGdiObj = NULL;
		dwType = REST_BITMAP;
		nWidth = 0;
		nHeight = 0;

		_tcscpy(szResImageName,_T(""));
		bHorzSplit = TRUE;
		nLength = -1;
		nCount = -1;
		nIndex = -1;
	}
	
}ResInfo,* PRESINFO;

//////////////////////////////////////////////////////////////////////////

//����Ƥ��
//SkinFile		:Ƥ��·��,ע�������*.ssk,Ҳ������Ƥ��Ŀ¼�е�INI�ļ�.
//bFromIni		:�ò���ָ��Ƥ���ļ��Ǵ�*.ssk��ȡ,���Ǵ�INI�ļ���ȡ.
SKINPPWTL_API BOOL  skinppLoadSkin(TCHAR* szSkinFile,BOOL bFromIni = FALSE);

SKINPPWTL_API BOOL skinppLoadSkinFromRes(HINSTANCE hInstance,LPCTSTR szResourceName,
										 LPCTSTR szResourceType,TCHAR* szSkinFileName);

//�Ƴ�Ƥ��
SKINPPWTL_API BOOL  skinppRemoveSkin();

//�˳�����⣬����������
SKINPPWTL_API BOOL  skinppExitSkin();

//����ListBox�ؼ����Ի���Ϣ
//hWnd			: ListBox�ؼ��ľ��
//nIndex		: Item�������
//pListBoxItem	: Item���Ի��Ľṹ��Ϣ
SKINPPWTL_API void skinppSetListBoxItemDrawInfo(HWND hWnd,int nIndex,struct ListBoxItem* pListBoxItem);

//��û������ϵͳ��ɫ
//nColorIndex	: Ҫ��ȡ����ɫ����
SKINPPWTL_API COLORREF skinppGetSkinSysColor(int nColorIndex);

//���WindowsϵͳĬ�ϵ���ɫ
//nColorIndex	: Ҫ��ȡ����ɫ����
SKINPPWTL_API COLORREF skinppGetDefaultSysColor(int nColorIndex);

//hWnd			: �Ի��򴰿ڵľ��
//nResID		: �Ի�����ԴID
SKINPPWTL_API BOOL skinppSetWindowResID(HWND hWnd,int nResID);//[������]

SKINPPWTL_API BOOL skinppSetFreeDlgID(HWND hWnd,int nResID);

SKINPPWTL_API BOOL skinppSetSkinResID(HWND hWnd,int nResID);

//����ListHeader���ڵ�������Ϣ
//hWnd			: ListHeader�Ĵ��ھ��
//nSortColumn	: Ҫ��ListHeader������е�����
//bSortAscending: �Ƿ�Ϊ����
SKINPPWTL_API void skinppSetListHeaderSortInfo(HWND hWnd,int nSortColumn,BOOL bSortAscending = TRUE);

//�ڸ�����HDC��,ָ����Ӧ�Ļ������ͺ�״̬,����Ӧ�ľ��������н��л���.
//hdc			:Ŀ��DC
//rect			:��������
//eDrawType		:�������ͣ�Ŀǰ֧��SPLITTERBARHORZ,SPLITTERBARVERT,SPLITTERBARBORDER
//nState		:ѡ�����״̬
SKINPPWTL_API void  skinppDrawSkinObject(HDC hdc,RECT rect,DRAWTYPE eDrawType,int nState);

//ͨ����ԴID,�����Ӧ���͵�Ƥ����Դλͼ���
//nSkinObjType	: Ƥ������,Ŀǰ֧��	DIALOGTYPE,BUTTONTYPE,CHECKBOXTYPE,RADIOBOXTYPE
//nResID		: ��ԴID
//nState		: ״̬,��BUTTONTYPE,CHECKBOXTYPE,RADIOBOXTYPE��Ч
SKINPPWTL_API HBITMAP skinppGetResFromID(SKINOBJTYPE nSkinObjType,int nResID,int nState =0 );

//�����Ƿ��Լ����Ի��򱳾�,�÷���������Ҫ�Լ��Ա������д���������.
//hWnd			: �Ի���ľ��
//bErase		: TRUE Ϊ�Լ�������,FALSE ΪSkin++��,���û�е��ø÷���,Skin++�����Ի��򱳾�.
SKINPPWTL_API void skinppSetDialogEraseBkgnd(HWND hWnd,BOOL bErase);

//���öԻ��򱳾��Ƿ�����ӿؼ�����
//hWnd			: �Ի�����
//bNoClip		: TRUEΪ����Ҫ����,FALSEΪ��Ҫ��������
//bAllChild		: TRUEΪ�ô���������ӶԻ��򶼼���.
SKINPPWTL_API void skinppSetDialogBkClipRgn(HWND hWnd,BOOL bClip,BOOL bAllChild = TRUE);

//ͨ��Ƥ����Դ���ƻ��Ƥ����Դ��λͼ
//szName		: Ƥ����Դ����
//HBITMAP		: ������Դ�е�λͼ
SKINPPWTL_API HBITMAP skinppGetBitmapRes(LPCTSTR szName);

//ͨ����Դ����ȡ��Դ���ڴ�ָ��
//szName		: ��Դ����
//nSize			: ��Դ��С
//pByte			: ����ֵ���ɹ����ط�NULL��ʧ�ܷ���NULL
SKINPPWTL_API BYTE*  skinppGetSkinResource(LPCTSTR szName,int& nSize);

//ͨ��Ƥ����Դ�����ƻ��λͼ�������������ֵ
//szName		: Ƥ����Դ����
//nTopHeight	: ���ز�������Ķ���
//nBottomHeight : ���ز�������ĵ׸�
//nLeftWidth	: ���ز�����������
//nRightWidth	: ���ز���������ҿ�
SKINPPWTL_API BOOL skinppGetBitmapResRect(LPCTSTR szName,int& nTopHeight,int& nBottomHeight,
												   int& nLeftWidth,int& nRightWidth);

//���ô����Ի��Ƿ��Լ�������,�÷��������Ի�������Ҫ�Լ�����������
//hWnd			: Ҫ�Ի��Ĵ��ھ��
//bCustomDraw	: TRUEΪ�Լ������Ի�,FALSEΪ����Skin++�����Ի�
SKINPPWTL_API void skinppSetCustomDraw(HWND hWnd,BOOL bCustomDraw);

//���ò˵���Ƥ����ʶ
//hWnd			: ӵ�в˵��Ĵ��ھ��
//nSkinObjectID	: �˵�Ƥ���ı�ʶ
SKINPPWTL_API void skinppSetMenuSkinObjectID(HWND hWnd,int nSkinObjectID);

//�����Ƿ���Ի��˵����л���
//bSkin			: TRUEΪ����
SKINPPWTL_API void skinppSetSkinOwnerMenu(BOOL bSkin);

//�Բ˵����л�������
//hMenu   : �뻻���Ĳ˵����
//bNoSkin : �Ƿ񻻷�,TRUEΪ������,FALSEΪ����
SKINPPWTL_API void  skinppSetDrawMenu(HMENU hMenu,BOOL bNoSkin);
	
//��ָ���Ĵ���ȥ��Ƥ��,���ұ�֤�����ٱ�����,��ʹʹ��SetSkinHwndҲ���ỻ��.
//hWnd			: ָ���Ĵ��ھ��
//bChildNoSkin	: �Ƿ�Ըô����е��Ӵ���ȥ��Ƥ��
SKINPPWTL_API void skinppSetNoSkinHwnd(HWND hWnd,BOOL bChildNoSkin = TRUE);

//��ָ���Ĵ��ڽ��л���
//hWnd			: ָ���Ĵ��ھ��
//szClassName	: Ҫ���໯��Skin���� WC_DIALOGBOX/WC_CONTROLBAR��
SKINPPWTL_API void skinppSetSkinHwnd(HWND hWnd,LPCTSTR szClassName = NULL);

//��ָ���Ĵ�����ʱȥ��Ƥ��,����ͨ��SetSkinHwnd�����ٴλ���
SKINPPWTL_API void skinppRemoveSkinHwnd(HWND hWnd);

//�Ƕ�SetNoSkinHwnd�Ľ�һ������,���Խ��ʹ��SetNoSkinHwnd�����Debug��Ķ��Դ�
#define SETNOSKINHWND(x) {\
    HWND w=(x).UnsubclassWindow();\
	skinppSetNoSkinHwnd(w);\
	(x).SubclassWindow(w);\
} 

//�Ƕ�RemoveSkinHwnd�Ľ�һ������,���Խ��ʹ��RemoveSkinHwnd�����Debug��Ķ��Դ�
#define REMOVESKINHWND(x){\
    HWND w=(x).UnsubclassWindow();\
	skinppRemoveSkinHwnd(w);\
	(x).SubclassWindow(w);\
}

SKINPPWTL_API HGDIOBJ skinppGetResFromID(PRESINFO pResInfo);


#endif //_SKINPPWTL_H_

