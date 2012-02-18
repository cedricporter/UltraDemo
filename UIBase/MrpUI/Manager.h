/*
** ���ߣ�����
** ˵����
**
*/
#pragma once



#include "Def.h"
#include "Base.h"
#include "InfoUIElement.h"
#include "Elements.h"
#include "MrpEditorData.h"
#include "..\MrpEditorParser\MrpStateParser.h"
#include "..\MrpEditorParser\FSMConfigFile.h"

#pragma warning ( disable: 4482 )	// �ر�����SelectedDrawingType::Tag�ľ���

typedef std::function< int( StateElement*, StateElement*, PredElement* ) > AddPredCallBackType;
typedef std::function< InfoUIElement*( int, int ) >	ClickCallBackType;
typedef std::function< int( InfoUIElement* ) >		AddStateCallBackType;
typedef std::function< int( CDC* ) >				DrawCallBackType;
typedef std::function< void() >						RefreshPropWndCBType;
typedef std::vector< CETString >					ListElementType;


/*
** ���֣�struct SelectedDrawingType
** ˵����DrawManager�б���ĵ�ǰ��������
**
*/
struct SelectedDrawingType
{
	enum Tag { STATE, PRED, LOCALPRED };

	Tag					tag;
	ListElementType*	params;
};




/*
** ���֣�InfoMananger
** ˵��������Action��ȫ��Predicate�;ֲ�Predicateӳ���
**
*/
class InfoMananger
{
public:
	typedef std::vector< CETString >			ListElementType;
	typedef std::vector< ListElementType >		ListTpye;
	typedef std::map< CETString, ListTpye >		MapType;

private:
	std::list< StateNode* >* m_pStateList;	// �����Parser���ص�Action�;ֲ�ν���б�
	std::list< StateNode* >* m_pPredList;	// �����Parser���ص�ȫ��ν���б�

	ListTpye	m_ActionList;				// Action�б�ÿһ����¼��vector<string>�����е�һλ�����֣�������ǲ���
	ListTpye	m_PredList;					// ȫ��ν���б�ÿһ����¼��vector<string>�����е�һλ�����֣�������ǲ���
	MapType		m_LocalPredMap;				// ���������;ֲ�ν�ʵ�ӳ���

	static InfoMananger* m_pInstance;		// InfoMananger��ʵ�� 

private:
	InfoMananger();

public:
	~InfoMananger();

	static InfoMananger& GetInstance()
	{
		if  (m_pInstance == NULL )
		{
			m_pInstance = new InfoMananger;
		}

		return *m_pInstance;
	}

	ListTpye& GetActionList()
	{
		return m_ActionList;
	}

	ListTpye& GetPredList()
	{
		return m_PredList;
	}

	MapType& GetLocalPredMap()
	{
		return m_LocalPredMap;
	}

};




/*
** ���֣�class DrawManager
** ˵������View�Ľ����㡣
**		ͨ��DrawManager��class Data������
**		����ѡ�л������ͺ󣬿�����Ӧ�����Ӧ����Ϣ��
**		�������ʱ���ߣ��ͱ�ѡ�����
**		
*/
class DrawManager
{
public:
	DrawManager();
	~DrawManager();

	/**** ������Ӧ���� ****/
	int OnDelete();
	int OnLButtonDown( int x, int y );	// ��Ӧ����������	
	int OnLButtonUp( int x, int y );	// ��Ӧ��굯��
	int OnMouseMove( int x, int y );	// ��Ӧ����ƶ�
	void OnUpdateSelectedElement();
		
	int Draw( CDC* pDC );				// ��ͼ

	/**** �������� ****/
	// ���õ�ǰѡ�е�����
	int SetSelectedDrawingElementType( SelectedDrawingType* selectedElement )		
	{ 
		int ret = FALSE;
		m_pSelectedDrawingType = selectedElement; 		
		if ( m_pSelectedDrawingType )	// Ϊ��ʱ��StateElement��������
		{ m_pTempStateForDrawingJumpPred->SetName( (*m_pSelectedDrawingType->params)[ 0 ] ); ret = TRUE;}
		return ret;
	}
	void SetRefreshPropListCB( const RefreshPropWndCBType& functor ) { m_RefreshPropWndCB = functor; }


	InfoUIElement* GetCurrentSelectedElement() const
	{ return m_pSelectedElememt; }

private:
	void AddInfoToElement( InfoUIElement* element );	// ��ѡ��Ԫ����Ϣ��Ϊ��ǰ���Ƶ�Ԫ��������ֺͲ���
	int TestAndDoAddPredElement( int x, int y );
	int TestAndDoAddStateElement( int x, int y );


private:
	InfoUIElement*			m_pLastSelectedElememt;			// �ϴ�ѡ�е�Ԫ��
	InfoUIElement*			m_pSelectedElememt;				// ��ǰѡ�е�Ԫ��
	InfoUIElementAnchor*	m_pElementAnchor;				// ѡ��Ԫ������
	PredElement*			m_pDrawingPredElement;			// ��ǰ���Ƶ�PredElement
	SelectedDrawingType*	m_pSelectedDrawingType;			// ��ǰѡ�еĻ�������	
	StateElement*			m_pTempStateForDrawingJumpPred;	// Ϊ�˻���PredElementʱ����Ƥ��Ч�������õ���ʱTarget

	/*** ����Data��Container�Ļص�����***/
	AddPredCallBackType		m_AddPredCB;		// ���ν��PredElement�Ļص�����
	AddStateCallBackType	m_AddStateCB;		// ���״̬StateElement�Ļص�����
	ClickCallBackType		m_ClickCB;			// ���ػ��е�Ԫ�أ�δ���з���NULL
	DrawCallBackType		m_DrawCB;			// Container��Draw�Ļص�
	RefreshPropWndCBType	m_RefreshPropWndCB;	// �������Դ��Ļص�	

	POINT m_lastPoint;		// �������StateElementʱ�����������϶�ʱ�������������
	bool m_bDragMoving;		// ����Ƿ�ǰ�����϶�״̬StateElement
	bool m_bDrawingLine;	// ����Ƿ�ǰ���ڻ���ν������PredElement
	int	m_stateWidth;		// ״̬StateElement�Ŀ��
	int m_stateHeight;		// ״̬StateElement�ĸ߶�

	friend class GlobalMananger;
};




/*
** ���֣�GlobeMananger
** ˵����DrawManager�� Data�� InfoElementContainer��GlobeMananger������
**		������Ϊ	DrawManager ���Data��InfoElementContainer�Ļص�����
*/
class GlobalMananger
{
private:
	DrawManager*			m_pDrawManager;		// �ӿڲ�
	Data*					m_pData;			// ���ݲ�
	InfoElementContainer*	m_pContainer;		// ���ֲ㣨UIElement������
	FSMConfigFile*			m_pFSMConfigFile;	// ͼ�κ��ı�֮���ת����

	static GlobalMananger*	m_pInstance;	// GlobalMananger��ʵ��

	ULONG_PTR				m_gdiplusToken; // Gdiplus��Token

	InfoUIElementDeletedFromDataSourceCBType m_DeleteFromSourceCB;	// �ṩ��Ԫ�ص���ɾ���ص�

public:
	struct SaveFileStruct
	{
		typedef std::function< int( const CETString& ) > SAVEFILEFUNC;

		SAVEFILEFUNC* pFunc;
		CETString filename;
	};

private:
	GlobalMananger();
	~GlobalMananger();
	
	void AddHandlers();	// ΪDrawManagerע��ص�����

public:
	static GlobalMananger& GetInstance()
	{
		if ( ! m_pInstance )
		{
			m_pInstance = new GlobalMananger;
		}

		return *m_pInstance;
	}

	// ��ȡDrawManager��ָ��
	DrawManager* GetDrawManager() { return m_pDrawManager; }

	// �����߳��б����ļ�
	int SaveFSMConfigFile( const CETString& filename );

	// ���������ļ����߳�
	static UINT SaveFileThreadFunc( LPVOID param );
};

