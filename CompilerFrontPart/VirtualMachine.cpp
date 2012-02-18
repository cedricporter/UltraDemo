/*
** 作者：华亮
** 说明：
**
*/

#include "StdAfx.h"
#include "VirtualMachine.h"
#include <windows.h>
#include <string>
#include <list>
#include <algorithm>

VirtualMachine::VirtualMachine() : m_currentNode( NULL )
{
	Reset();
}

void VirtualMachine::LoadFromString( const CETString& codeString )
{
	Reset();
	m_parser.LoadFromString( codeString );
}

void VirtualMachine::Reset()
{
	m_currentNode = NULL;
	m_Stack.clear();	
	while ( m_EnvStack.size() > 0 )	m_EnvStack.pop();
	m_parser.Reset();

	StackElementType e;
	m_Stack.push_back( e );
}

void VirtualMachine::Compile()
{
	// delete the original tree
	try
	{
		Reset();
		m_parser.Parse();
		m_currentNode = (StmtSeq*)m_parser.GetSyntaxTree();
	}
	catch ( char* eStr )
	{
		MessageBoxA( NULL, eStr, "ERROR!", MB_OK );
	}

}

bool VirtualMachine::Run()
{
	while ( IsReady() )
	{
		Step();
	}
	return true;
}

bool VirtualMachine::Step()
{
	if ( ! m_currentNode ) return false;

	Stmt* s1 = m_currentNode->m_pStmt1;
	Stmt* s2 = m_currentNode->m_pStmt2;
	
	if ( typeid( *s1 ) == typeid( SetStmt ) )			// 赋值语句
	{
		RunSetStmt(s1, s2);
	}	
	else if ( typeid( *s1 ) == typeid( IfStmt ) )		// if语句
	{
		RunIfStmt(s2, s1);		
	}	
	else if ( typeid( *s1 ) == typeid( IfElseStmt ) )	// if-else语句
	{
		RunIfElseStmt(s2, s1);
	}	
	else if ( typeid( *s1 ) == typeid( WhileStmt ) )	// while语句
	{
		RunWhileStmt(s1, s2);
	}
	else if ( typeid( *s1 ) == typeid( BreakStmt ) )	// break语句
	{
		RunBreakStmt(s1);
	}
	else if ( typeid( *s1 ) == typeid( DeclareStmt ) )	// 变量声明语句
	{
		RunDeclareStmt(s1, s2);
	}

	return RecoverEnvironment();
}

void VirtualMachine::Pop()
{
	m_EnvStack.pop();
	m_Stack.pop_back();
}

void VirtualMachine::Push( Stmt* pStmt )
{
	m_EnvStack.push( (StmtSeq*)pStmt );

	StackElementType stack;
	m_Stack.push_back( stack );
}

void VirtualMachine::RunSetStmt( Stmt* s1, Stmt* s2 )
{
	SetStmt* pset = (SetStmt*)s1;
	Word* word = (Word*)pset->m_id.m_pTok;
	Id& id = pset->m_id;

	// 从栈顶往下找
	for ( auto iter = m_Stack.rbegin(); iter != m_Stack.rend(); iter++ )
	{
		if ( iter->find( word->m_lexeme ) != iter->end() )
		{
			ValueNode& node = (*iter)[ word->m_lexeme ];

			if ( node.pType->m_lexeme == "void*" )
			{
				ValueNode n = CalcExp( pset->m_pExpr );
				((int*)node.Val.pVal)[ CalcExp( id.m_arrayCount.front() ).Val.iVal ] = n.Val.iVal;
			}
			else
			{
				node = CalcExp( pset->m_pExpr );
			}

			m_currentNode = (StmtSeq*)s2;	// 下一个节点
			return;
		}
	}



	throw "Not defined symbol";

}

void VirtualMachine::RunIfStmt( Stmt* s2, Stmt* s1 )
{
	Push(s2);

	IfStmt* pIf = (IfStmt*)s1;
	ValueNode bExp = CalcExp( pIf->m_pExpr );
	if ( bExp.Val.bVal )
	{
		m_currentNode = (StmtSeq*)pIf->m_pStmt;
	}
	else
	{
		m_currentNode = (StmtSeq*)s2;	// 下一个节点
		Pop();	
	}
}

void VirtualMachine::RunIfElseStmt( Stmt* s2, Stmt* s1 )
{
	Push(s2);

	IfElseStmt* pIf = (IfElseStmt*)s1;
	ValueNode bExp = CalcExp( pIf->m_pExpr );
	if ( bExp.Val.bVal )
	{
		m_currentNode = (StmtSeq*)pIf->m_pIfStmt;
	}
	else
	{
		m_currentNode = (StmtSeq*)pIf->m_pElseStmt;
	}
}

void VirtualMachine::RunWhileStmt( Stmt* s1, Stmt* s2 )
{
	WhileStmt* pWhile = (WhileStmt*)s1;
	Push( (StmtSeq*)m_currentNode );	// 压进while

	ValueNode bExpr = CalcExp( pWhile->m_pExpr );
	if ( bExpr.Val.bVal )
	{
		m_currentNode = (StmtSeq*)pWhile->m_pStmt;
	}
	else
	{
		Pop();							// 跳出while，从栈弹出while
		m_currentNode = (StmtSeq*)s2;	// 下一个节点
	}
}

bool VirtualMachine::RecoverEnvironment()
{
	if ( m_currentNode == NULL )
	{
		if ( m_EnvStack.size() > 0 )
		{
			m_currentNode = m_EnvStack.top();
			Pop();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void VirtualMachine::RunBreakStmt( Stmt* s1 )
{
	BreakStmt* pBreak = (BreakStmt*)s1;
	StmtSeq* pStmt = m_EnvStack.top();
	Pop();
	while ( typeid( *pStmt->m_pStmt1 ) != typeid( WhileStmt ) )
	{
		pStmt = m_EnvStack.top();
		Pop();
	}
	m_currentNode = (StmtSeq*)pStmt->m_pStmt2;
}

void VirtualMachine::RunDeclareStmt( Stmt* s1, Stmt* s2 )
{
	DeclareStmt* pDecl = (DeclareStmt*)s1;

	//pDecl->m_type;
	//pDecl->m_setStmtList;

	StackElementType& stackTop = m_Stack.back();

	std::for_each( pDecl->m_setStmtList.begin(), pDecl->m_setStmtList.end(), [&]( SetStmt* pSet )
	{
		if ( stackTop.find( dynamic_cast<Word*>(pSet->m_id.m_pTok)->m_lexeme ) == stackTop.end() )
		{
			if ( pSet->m_id.m_arrayCount.size() > 0 )
			{
				if ( pSet->m_id.m_pType->m_lexeme == "int" )
				{
					ValueNode& node = stackTop[ dynamic_cast<Word*>(pSet->m_id.m_pTok)->m_lexeme ];
					node.Val.pVal = new int[ CalcExp( pSet->m_id.m_arrayCount.front() ).Val.iVal ];
					node.pType = Type::Pointer;
				}
			}
			else
			{
				stackTop[ dynamic_cast<Word*>(pSet->m_id.m_pTok)->m_lexeme ] = pSet->m_pExpr == NULL ? 0 : CalcExp(pSet->m_pExpr);
			}
		}
	});

	m_currentNode = (StmtSeq*)s2;	// 下一个节点
}

ValueNode VirtualMachine::CalcExp( Expr* node )
{
	// 常量
	if ( typeid( *node ) == typeid( Constant ) )
	{
		return ((Num*)((Constant*)node)->m_pTok)->m_Val;
	}

	// 函数调用
	if ( typeid( *node ) == typeid( CallExpr ) )
	{
		if ( ((Word*)((Expr*)((Id*)&((CallExpr*)node)->m_id )->m_pTok))->m_lexeme == "call" )
		{
			std::string str;
			char temp[ 20 ];
			str += _itoa_s( ((Num*)((CallExpr*)node)->m_params.front()->m_pTok)->m_Val, temp, 10 );
			str += " ";
			str += _itoa_s( ((Num*)((CallExpr*)node)->m_params.back()->m_pTok)->m_Val, temp, 10 );
			MessageBoxA( NULL, 
				str.c_str(),
				((Word*)((Expr*)((Id*)&((CallExpr*)node)->m_id )->m_pTok))->m_lexeme.c_str(),				
				MB_OK );

			return 0;
		}
	}

	// 变量
	if ( node->m_pTok && typeid( *node->m_pTok ) == typeid( Word ) )
	{	
		// 从栈顶往下找
		for ( auto iter = m_Stack.rbegin(); iter != m_Stack.rend(); iter++ )
		{
			if ( iter->find( ((Word*)node->m_pTok)->m_lexeme ) != iter->end() )
			{
				if ( typeid( *node ) == typeid( Id ) )
				{
					Id* id = (Id*)node;
					if ( id->m_arrayCount.size() > 0 )
					{
						int* p = (int*)(*iter)[ ((Word*)node->m_pTok)->m_lexeme ].Val.pVal;
						return p[ CalcExp( id->m_arrayCount.front() ).Val.iVal ];
					}
				}

				return (*iter)[ ((Word*)node->m_pTok)->m_lexeme ];
			}
		}
	}

	// 二元运算
	ValueNode left, right;
	if ( node && ( typeid( *node ) == typeid( Arith ) || typeid( *node ) == typeid( Relation )))
	{
		switch( ((Arith*)node)->m_pTok->m_Tag )
		{
		case Tag::PLUS:
			left = CalcExp( ((Arith*)node)->m_pExpr1 );
			right = CalcExp( ((Arith*)node)->m_pExpr2 );
			return left + right;
		case Tag::MINUS:
			left = CalcExp( ((Arith*)node)->m_pExpr1 );
			right = CalcExp( ((Arith*)node)->m_pExpr2 );
			return left - right;
		case Tag::TIMES:
			left = CalcExp( ((Arith*)node)->m_pExpr1 );
			right = CalcExp( ((Arith*)node)->m_pExpr2 );
			return left * right;
		case Tag::OVER:
			left = CalcExp( ((Arith*)node)->m_pExpr1 );
			right = CalcExp( ((Arith*)node)->m_pExpr2 );
			return left / right;
		case Tag::EQ:
			left = CalcExp( ((Arith*)node)->m_pExpr1 );
			right = CalcExp( ((Arith*)node)->m_pExpr2 );
			return left == right;
		case Tag::NE:
			left = CalcExp( ((Arith*)node)->m_pExpr1 );
			right = CalcExp( ((Arith*)node)->m_pExpr2 );
			return left != right;
		case Tag::LT:
			left = CalcExp( ((Arith*)node)->m_pExpr1 );
			right = CalcExp( ((Arith*)node)->m_pExpr2 );
			return left < right;
		case Tag::GT:
			left = CalcExp( ((Arith*)node)->m_pExpr1 );
			right = CalcExp( ((Arith*)node)->m_pExpr2 );
			return left > right;
		case Tag::LE:
			left = CalcExp( ((Arith*)node)->m_pExpr1 );
			right = CalcExp( ((Arith*)node)->m_pExpr2 );
			return left <= right;		
		case Tag::GE:
			left = CalcExp( ((Arith*)node)->m_pExpr1 );
			right = CalcExp( ((Arith*)node)->m_pExpr2 );
			return left >= right;
		default:
			MessageBoxA( NULL, _T("请迅速解决表达式处理问题！"), _T(""), MB_OK );
			break;
		}
	}

	return 0;
}
