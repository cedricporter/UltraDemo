// AbstactSyntaxTree.cpp : 实现文件
//

#include "stdafx.h"
#include "AbstactSyntaxTree.h"


// AbstactSyntaxTree

IMPLEMENT_DYNAMIC(AbstactSyntaxTree, CDockablePane)

AbstactSyntaxTree::AbstactSyntaxTree()
{

}

AbstactSyntaxTree::~AbstactSyntaxTree()
{
}


BEGIN_MESSAGE_MAP(AbstactSyntaxTree, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// AbstactSyntaxTree 消息处理程序




int AbstactSyntaxTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_synTree.Create( WS_CHILD | WS_VISIBLE | TVS_HASLINES, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}
	
	

	m_synTree.DeleteAllItems();

	//HTREEITEM hroot=m_synTree.InsertItem("清华大学",1,0,TVI_ROOT);//添加一级结点
	//HTREEITEM h1=m_synTree.InsertItem("机械工程学院",1,0,hroot);//添加二级结点
	//HTREEITEM h2=m_synTree.InsertItem("理学院",1,0,hroot);//添加二级结点
	//HTREEITEM h3=m_synTree.InsertItem("经济管理学院",1,0,hroot);//添加二级结点
	//HTREEITEM h11=m_synTree.InsertItem("机械工程系",1,0,h1);//添加三级结点
	//HTREEITEM h12=m_synTree.InsertItem("精仪系 ",1,0,h1);//添加三级结点
	//HTREEITEM h13=m_synTree.InsertItem("汽车系",1,0,h1);//添加三级结点
	//HTREEITEM h21=m_synTree.InsertItem("应用物理系",1,0,h2);//添加三级结点
	//HTREEITEM h22=m_synTree.InsertItem("数学系",1,0,h2);//添加三级结点	

	m_synTree.SetBkColor(RGB(255,255,255));//设置树形控件的背景色
	m_synTree.SetTextColor(RGB(127,0,0));//设置文本颜色

	PostMessage(WM_SIZE);
	return 0;
}


void AbstactSyntaxTree::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rectClient,rectCtrlTree;
	GetClientRect(rectClient);
	m_synTree.SetWindowPos( NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER );
}


// 加载语法树
void AbstactSyntaxTree::LoadSyntaxTree( const std::map<std::string, Stmt*>* treeMap )
{
	m_synTree.DeleteAllItems();
	// Print Syntax Tree
	for ( auto iter = treeMap->begin(); iter != treeMap->end(); iter ++ )
	{
		HTREEITEM root = InsertItem( "Function " + iter->first, TVI_ROOT );

		PrintTree( iter->second, root );
	}

	HTREEITEM hItem;
	hItem= m_synTree.GetFirstVisibleItem();
	while (hItem != NULL)
	{
		m_synTree.Expand(hItem,TVE_EXPAND);
		hItem = m_synTree.GetNextItem(hItem, TVGN_NEXTVISIBLE);
	}

}

void AbstactSyntaxTree::Log( const std::string& log )
{
	m_printTreeLog += log;
	m_printTreeLog += '\n';
}

HTREEITEM AbstactSyntaxTree::PrintExpr( Expr* expr, HTREEITEM ctrlRoot )
{
	if ( expr == NULL )	return NULL;

	if ( typeid( *expr ) == typeid( ExprSeq ) )
	{
		ExprSeq* seq = dynamic_cast<ExprSeq*>( expr );
		PrintExpr( seq->m_pExpr1, ctrlRoot );
		PrintExpr( seq->m_pExpr2, ctrlRoot );
	}		
	else if ( typeid( *expr ) == typeid( Id ) )
	{
		Id* id = dynamic_cast<Id*>( expr );

		HTREEITEM item = InsertItem( dynamic_cast<Word*>( id->m_pTok )->m_lexeme.c_str(), ctrlRoot );

	}
	else if ( typeid( *expr ) == typeid( ArrayAccess ) )
	{
		//Log( "ArrayAccess" );
		ArrayAccess* access = dynamic_cast<ArrayAccess*>( expr );
		HTREEITEM item = InsertItem( "ArrayAccess", ctrlRoot );

		PrintExpr( &access->m_array, item );
		PrintExpr( access->m_index, item );
	}
	else if ( typeid( *expr ) == typeid( CallExpr ) )
	{
		//Log( "CallExpr" );
		CallExpr* call = dynamic_cast<CallExpr*>( expr );

		HTREEITEM item = InsertItem( "Call " + dynamic_cast<Word*>( call->m_id.m_pTok )->m_lexeme, ctrlRoot );
		
	}
	else if ( typeid( *expr ) == typeid( Arith ) )
	{
		//Log( "Arith" );
		Arith* arith = dynamic_cast<Arith*>( expr );

		HTREEITEM item = InsertItem( arith->m_pTok->ToString(), ctrlRoot );

		PrintExpr( arith->m_pExpr1, item );
		PrintExpr( arith->m_pExpr2, item );
	}
	else if ( typeid( *expr ) == typeid( Unary ) )
	{
		//Log( "Unary" );
		Unary* unary = dynamic_cast<Unary*>( expr );

		HTREEITEM item = InsertItem( unary->m_pTok->ToString(), ctrlRoot );

		PrintExpr( unary->m_pExpr, item );
	}
	else if ( typeid( *expr ) == typeid( Operation ) )
	{
		Log( "Operation" );
	}
	else if ( typeid( *expr ) == typeid( Constant ) )
	{
		//Log( "Constant" );
		Constant* constant = dynamic_cast<Constant*>( expr );
		
		HTREEITEM item = InsertItem( constant->m_pTok->ToString(), ctrlRoot );

	}
	else if ( typeid( *expr ) == typeid( AndOperator ) || typeid( *expr ) == typeid( OrOperator ) )
	{
		//Log( "AndOperator" );
		Logical* logic = dynamic_cast<Logical*>( expr );

		HTREEITEM item = InsertItem( logic->m_pTok->ToString(), ctrlRoot );

		PrintExpr( logic->m_pExpr1, item );
		PrintExpr( logic->m_pExpr2, item );
		
	}
	else if ( typeid( *expr ) == typeid( Logical ) )
	{
		//Log( "Logical" );
		Logical* logic = dynamic_cast<Logical*>( expr );
		logic->m_pTok->ToString();

	}
	else if ( typeid( *expr ) == typeid( Relation ) )
	{
		Relation* rel = dynamic_cast<Relation*>( expr );

		HTREEITEM item = InsertItem( rel->m_pTok->ToString(), ctrlRoot );

		PrintExpr( rel->m_pExpr1, item );
		PrintExpr( rel->m_pExpr2, item );
	}
	//else if ( typeid( *expr ) == typeid())

	return NULL;
}

HTREEITEM AbstactSyntaxTree::PrintTree( Stmt* root, HTREEITEM ctrlRoot )
{
	if ( root == NULL )		return NULL;

	if ( typeid( *root ) == typeid( StmtSeq ) )
	{
		StmtSeq* node = dynamic_cast<StmtSeq*>( root );

		PrintTree( node->m_pStmt1, ctrlRoot );
		PrintTree( node->m_pStmt2, ctrlRoot );
	}
	else if ( typeid( *root ) == typeid( ExprStmt ) )
	{
		ExprStmt* exprStmt = dynamic_cast<ExprStmt*>( root );
		PrintExpr( exprStmt->m_pExpr, ctrlRoot );
	}
	else if ( typeid( *root ) == typeid( IfStmt ) )
	{
		HTREEITEM item = InsertItem( "if", ctrlRoot );
		HTREEITEM exprItem = InsertItem( "conditional-expr", item );

		//Log( "if" );
		IfStmt* ifStmt = dynamic_cast<IfStmt*>( root );
		PrintExpr( ifStmt->m_pExpr, exprItem );
		PrintTree( ifStmt->m_pStmt, item );
	}
	else if ( typeid( *root ) == typeid( IfElseStmt ) )
	{
		HTREEITEM item = InsertItem( "ifelse", ctrlRoot );

		HTREEITEM exprItem = InsertItem( "conditional-expr", item );

		//Log( "ifelse" );
		IfElseStmt* ifElseStmt = dynamic_cast<IfElseStmt*>( root );
		PrintExpr( ifElseStmt->m_pExpr, exprItem );
		PrintTree( ifElseStmt->m_pIfStmt, item );
		PrintTree( ifElseStmt->m_pElseStmt, item );
	}
	else if ( typeid( *root ) == typeid( WhileStmt ) )
	{
		HTREEITEM item = InsertItem( "while", ctrlRoot );

		HTREEITEM exprItem = InsertItem( "conditional-expr", item );

		//Log( "while" );
		WhileStmt* whileStmt = dynamic_cast<WhileStmt*>( root );
		PrintExpr( whileStmt->m_pExpr, exprItem );
		PrintTree( whileStmt->m_pStmt, item );
	}
	else if ( typeid( *root ) == typeid( WriteStmt ) )
	{
		HTREEITEM item = InsertItem( "write", ctrlRoot );

		//Log( "write" );
		WriteStmt* writeStmt = dynamic_cast<WriteStmt*>( root );
		PrintExpr( writeStmt->m_pExpr, item );
	}
	else if ( typeid( *root ) == typeid( ReturnStmt ) )
	{
		HTREEITEM item = InsertItem( "return", ctrlRoot );

		//Log( "return " );
		ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>( root );
		PrintExpr( returnStmt->m_pExpr, item );
	}

	return NULL;
}

HTREEITEM AbstactSyntaxTree::InsertItem( const std::string &treeNodeName, HTREEITEM ctrlRoot )
{
	HTREEITEM item = m_synTree.InsertItem( treeNodeName.c_str(), 1, 0, ctrlRoot );
	
	//m_synTree.Expand( item, TVE_EXPAND );
	
	return item;
}
