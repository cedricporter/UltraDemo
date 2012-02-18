/*
** 作者：华亮
** 说明：
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

#pragma warning ( disable: 4482 )	// 关闭引用SelectedDrawingType::Tag的警告

typedef std::function< int( StateElement*, StateElement*, PredElement* ) > AddPredCallBackType;
typedef std::function< InfoUIElement*( int, int ) >	ClickCallBackType;
typedef std::function< int( InfoUIElement* ) >		AddStateCallBackType;
typedef std::function< int( CDC* ) >				DrawCallBackType;
typedef std::function< void() >						RefreshPropWndCBType;
typedef std::vector< CETString >					ListElementType;


/*
** 名字：struct SelectedDrawingType
** 说明：DrawManager中保存的当前绘制类型
**
*/
struct SelectedDrawingType
{
	enum Tag { STATE, PRED, LOCALPRED };

	Tag					tag;
	ListElementType*	params;
};




/*
** 名字：InfoMananger
** 说明：保存Action、全局Predicate和局部Predicate映射表
**
*/
class InfoMananger
{
public:
	typedef std::vector< CETString >			ListElementType;
	typedef std::vector< ListElementType >		ListTpye;
	typedef std::map< CETString, ListTpye >		MapType;

private:
	std::list< StateNode* >* m_pStateList;	// 保存从Parser返回的Action和局部谓词列表
	std::list< StateNode* >* m_pPredList;	// 保存从Parser返回的全局谓词列表

	ListTpye	m_ActionList;				// Action列表，每一条记录是vector<string>，其中第一位是名字，后面的是参数
	ListTpye	m_PredList;					// 全局谓词列表，每一条记录是vector<string>，其中第一位是名字，后面的是参数
	MapType		m_LocalPredMap;				// Ａｃｔｉｏｎ和局部谓词的映射表

	static InfoMananger* m_pInstance;		// InfoMananger的实例 

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
** 名字：class DrawManager
** 说明：和View的交互层。
**		通过DrawManager和class Data交互。
**		设置选中绘制类型后，可以响应鼠标相应的消息。
**		会绘制临时连线，和被选中外框。
**		
*/
class DrawManager
{
public:
	DrawManager();
	~DrawManager();

	/**** 对外响应函数 ****/
	int OnDelete();
	int OnLButtonDown( int x, int y );	// 响应鼠标左键按下	
	int OnLButtonUp( int x, int y );	// 响应鼠标弹起
	int OnMouseMove( int x, int y );	// 响应鼠标移动
	void OnUpdateSelectedElement();
		
	int Draw( CDC* pDC );				// 绘图

	/**** 辅助函数 ****/
	// 设置当前选中的类型
	int SetSelectedDrawingElementType( SelectedDrawingType* selectedElement )		
	{ 
		int ret = FALSE;
		m_pSelectedDrawingType = selectedElement; 		
		if ( m_pSelectedDrawingType )	// 为临时的StateElement设置名字
		{ m_pTempStateForDrawingJumpPred->SetName( (*m_pSelectedDrawingType->params)[ 0 ] ); ret = TRUE;}
		return ret;
	}
	void SetRefreshPropListCB( const RefreshPropWndCBType& functor ) { m_RefreshPropWndCB = functor; }


	InfoUIElement* GetCurrentSelectedElement() const
	{ return m_pSelectedElememt; }

private:
	void AddInfoToElement( InfoUIElement* element );	// 从选中元素信息中为当前绘制的元素添加名字和参数
	int TestAndDoAddPredElement( int x, int y );
	int TestAndDoAddStateElement( int x, int y );


private:
	InfoUIElement*			m_pLastSelectedElememt;			// 上次选中的元素
	InfoUIElement*			m_pSelectedElememt;				// 当前选中的元素
	InfoUIElementAnchor*	m_pElementAnchor;				// 选中元素修饰
	PredElement*			m_pDrawingPredElement;			// 当前绘制的PredElement
	SelectedDrawingType*	m_pSelectedDrawingType;			// 当前选中的绘制类型	
	StateElement*			m_pTempStateForDrawingJumpPred;	// 为了绘制PredElement时的橡皮筋效果而设置的临时Target

	/*** 操纵Data和Container的回调函数***/
	AddPredCallBackType		m_AddPredCB;		// 添加谓词PredElement的回调函数
	AddStateCallBackType	m_AddStateCB;		// 添加状态StateElement的回调函数
	ClickCallBackType		m_ClickCB;			// 返回击中的元素，未击中返回NULL
	DrawCallBackType		m_DrawCB;			// Container的Draw的回调
	RefreshPropWndCBType	m_RefreshPropWndCB;	// 更新属性窗的回调	

	POINT m_lastPoint;		// 鼠标命中StateElement时保存的坐标和拖动时保存的最后的坐标
	bool m_bDragMoving;		// 标记是否当前正在拖动状态StateElement
	bool m_bDrawingLine;	// 标记是否当前正在绘制谓词连线PredElement
	int	m_stateWidth;		// 状态StateElement的宽度
	int m_stateHeight;		// 状态StateElement的高度

	friend class GlobalMananger;
};




/*
** 名字：GlobeMananger
** 说明：DrawManager， Data， InfoElementContainer由GlobeMananger创建，
**		在这里为	DrawManager 添加Data和InfoElementContainer的回调函数
*/
class GlobalMananger
{
private:
	DrawManager*			m_pDrawManager;		// 接口层
	Data*					m_pData;			// 数据层
	InfoElementContainer*	m_pContainer;		// 表现层（UIElement容器）
	FSMConfigFile*			m_pFSMConfigFile;	// 图形和文本之间的转换器

	static GlobalMananger*	m_pInstance;	// GlobalMananger的实例

	ULONG_PTR				m_gdiplusToken; // Gdiplus的Token

	InfoUIElementDeletedFromDataSourceCBType m_DeleteFromSourceCB;	// 提供给元素的自删除回调

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
	
	void AddHandlers();	// 为DrawManager注册回调函数

public:
	static GlobalMananger& GetInstance()
	{
		if ( ! m_pInstance )
		{
			m_pInstance = new GlobalMananger;
		}

		return *m_pInstance;
	}

	// 获取DrawManager的指针
	DrawManager* GetDrawManager() { return m_pDrawManager; }

	// 在新线程中保存文件
	int SaveFSMConfigFile( const CETString& filename );

	// 创建保存文件的线程
	static UINT SaveFileThreadFunc( LPVOID param );
};

