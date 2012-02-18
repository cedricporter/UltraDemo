/*
** ���ߣ�����
** ˵�����ʷ�������
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


//#pragma warning ( disable: 4355 )	// �ر�thisָ�뾯��

class Scanner
{
public:
	typedef std::function< int() >					HandlerFunctorType;
	typedef std::map< CETString, Word* >			MAPTYPE;
	typedef std::list< HandlerFunctorType* >		LISTTYPE;

	enum { // ����������ֵ
		CONTINUE = 0,	// ����������
		END = 1			// ����������
	};

private:
	Token*			m_currentToken;		// ���浱ǰToken
	CETString		m_tokenString;		// ��ǰToken���ַ���
	FileReader*		m_pFileReader;		// �ļ���ȡ��
	StateMachine*	m_pFSM;				// ״̬��
	MAPTYPE			m_reservedWords;	// ������ӳ���
	int				m_bFileOpened;		// ����ļ��Ƿ��Ѿ�����
	LISTTYPE		m_cbList;			// ����ص�����ָ��������ص�������ʱ�����Ļ�ֻ�ܷ���heap��

public:
	Scanner();
	~Scanner();

	int OpenFile( const CETString& filename ) // ���ļ�
	{ return m_bFileOpened = m_pFileReader->OpenFile( filename ); }

	int GetChar() { char c; m_tokenString += c = m_pFileReader->GetChar(); return c; }
	int GetNextChar() { char c = GetChar(); m_pFileReader->Next(); return c; }
	int NextChar()	{ m_pFileReader->Next(); char c = GetChar(); return c; }

	void Reset()							// ����Token
	{ m_currentToken = NULL; m_tokenString.clear(); }	
	Token* GetToken();						// �����ļ�����ȡ����Token

	void AddReservedWord();					// �����캯��������һ������ӱ�����
	void reserve( Word* w )					// ��ӵ�������
	{ m_reservedWords[ w->m_lexeme ] = w; }	

	int AddHandlers();						// �����캯��������һ����Ϊ�Զ�����Ӿ��
	int otherHandler( LPVOID param );		// ���ڴ�������״̬
	int commentHandler( LPVOID param );		// ���ڴ���ע��
	int whitespaceHandler( LPVOID param );	// ���ڴ���հ�
	int numHandler( LPVOID param );			// ���ڴ�������
	int IDHandler( LPVOID param );			// ���ڴ���ID�͹ؼ���
	int strHandler( LPVOID param );			// ���ڴ����ַ���
	int errorHandler( LPVOID );				// �쳣�����ӡ�����к�

	void PrintToken();						// ��ӡ��ǰToken
};




