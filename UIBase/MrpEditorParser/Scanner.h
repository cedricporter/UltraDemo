/*
** 作者：华亮
** 说明：词法器声明
**
*/

#pragma once

#include "Token.h"
#include "def.h"
#include "FileReader.h"
#include "StateMachine.h"

#include <iostream>
#include <map>
#include <sstream>
#include <cassert>
#include <list>

#define SCANNER_REPORT 1


//#pragma warning ( disable: 4355 )	// 关闭this指针警告

class Scanner
{
public:
	typedef std::function< int() >					HandlerFunctorType;
	typedef std::map< CETString, Word* >			MAPTYPE;
	typedef std::list< HandlerFunctorType* >		LISTTYPE;

	enum { // 处理函数返回值
		CONTINUE = 0,	// 处理完后继续
		END = 1			// 处理完后结束
	};

private:
	Token*			m_currentToken;		// 保存当前Token
	CETString		m_tokenString;		// 当前Token的字符串
	FileReader*		m_pFileReader;		// 文件读取器
	StateMachine*	m_pFSM;				// 状态机
	MAPTYPE			m_reservedWords;	// 保留字映射表
	int				m_bFileOpened;		// 标记文件是否已经开启
	LISTTYPE		m_cbList;			// 保存回调函数指针的链表，回调函数临时参数的话只能放在heap中

public:
	Scanner();
	~Scanner();

	int OpenFile( const CETString& filename ) // 打开文件
	{ return m_bFileOpened = m_pFileReader->OpenFile( filename ); }

	int GetChar() { char c; m_tokenString += c = m_pFileReader->GetChar(); return c; }
	int GetNextChar() { char c = GetChar(); m_pFileReader->Next(); return c; }
	int NextChar()	{ m_pFileReader->Next(); char c = GetChar(); return c; }

	void Reset()							// 重设Token
	{ m_currentToken = NULL; m_tokenString.clear(); }	
	Token* GetToken();						// 返回文件流中取出的Token

	void AddReservedWord();					// 被构造函数调用来一次性添加保留字
	void reserve( Word* w )					// 添加到保留字
	{ m_reservedWords[ w->m_lexeme ] = w; }	

	int AddHandlers();						// 被构造函数调用来一次性为自动机添加句柄
	int otherHandler( LPVOID param );		// 用于处理其他状态
	int commentHandler( LPVOID param );		// 用于处理注释
	int whitespaceHandler( LPVOID param );	// 用于处理空白
	int numHandler( LPVOID param );			// 用于处理数字
	int IDHandler( LPVOID param );			// 用于处理ID和关键字
	int strHandler( LPVOID param );			// 用于处理字符串
	int errorHandler( LPVOID );				// 异常，会打印错误行号

	void PrintToken();						// 打印当前Token
};




