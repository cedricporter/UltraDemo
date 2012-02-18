#pragma once

#include <map>
#include <string>
#include "TreeDef.h"
// AbstactSyntaxTree
using namespace ETCompiler;

class AbstactSyntaxTree : public CDockablePane
{
	DECLARE_DYNAMIC(AbstactSyntaxTree)

public:
	AbstactSyntaxTree();
	virtual ~AbstactSyntaxTree();

protected:
	CTreeCtrl	m_synTree;
	std::string m_printTreeLog;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// º”‘ÿ”Ô∑® ˜
	void LoadSyntaxTree( const std::map<std::string, Stmt*>* treeMap );


	void Log( const std::string& log );

	HTREEITEM PrintExpr( Expr* expr, HTREEITEM ctrlRoot );

	HTREEITEM PrintTree( Stmt* root, HTREEITEM ctrlRoot );

	HTREEITEM InsertItem( const std::string &treeNodeName, HTREEITEM ctrlRoot );

};


