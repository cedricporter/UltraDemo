#include "stdafx.h"
#include "SymbolTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ETCompiler
{

	//////////////////////////////////////////////////////////////////////////
	//
	// Class Table
	// 
	Table::Table( const std::string& name, bool bStruct /*= false */ )
		: m_paramTableWidth( -(long)(sizeof( long )) ), m_symbolNum( 0 ),
		m_tablename( name ), m_bStructTable( bStruct ), m_id( -1 )
	{
		if ( m_bStructTable )
		{
			m_tableWidth = 0;
		}
		else 
		{
			m_tableWidth = sizeof( dword );
		}
	}

	void Table::InsertParam( const Symbol& symbol )
	{
		Symbol& sym = m_paramSymbolList[ symbol.name ] = symbol;	// �������
		sym.offset = m_paramTableWidth;								// ��������ڷ��ű��е�ƫ��λ��
		m_paramTableWidth -= (symbol.GetWidth() ) ;	// ���¼�����ű���
	}

	void Table::InsertSymbol( const Symbol& symbol )
	{
		m_symbolNum++;											// ���Ÿ�����1
		Symbol& sym = m_symbolList[ symbol.name ] = symbol;		// �������
		if ( m_bStructTable )
		{
			sym.offset = m_tableWidth;								// ��������ڷ��ű��е�ƫ��λ��
			m_tableWidth += (symbol.GetWidth()) ;	// ���¼�����ű���
		}
		else
		{
			m_tableWidth += (symbol.GetWidth() ) ;	// ���¼�����ű���
			sym.offset = m_tableWidth;								// ��������ڷ��ű��е�ƫ��λ��
		}
	}

	Symbol* Table::GetSymbol( const std::string& name )
	{
		if ( m_symbolList.find( name ) != m_symbolList.end() ) 
		{
			return &m_symbolList[ name ];
		}
		if ( m_paramSymbolList.find( name ) != m_paramSymbolList.end() )
		{
			return &m_paramSymbolList[ name ];
		}

		return NULL;
	}

	std::size_t Table::GetTableSize()
	{
		return m_tableWidth - ( m_bStructTable ? 0 : sizeof( dword ) );
	}

	std::string& Table::GetName()
	{
		return m_tablename;
	}





	//////////////////////////////////////////////////////////////////////////
	//
	// Class Environment
	// 
	void Environment::AddTable( const std::string& name, bool bStruct /*= false */ )
	{
		Table table( name, bStruct );

		m_tableList[ name ] = table;
		m_pCurrentTable = &m_tableList[ name ];		
	}

	void Environment::InsertSymbol( const std::string tablename, const Symbol& symbol )
	{
		if ( m_tableList.find( tablename ) != m_tableList.end() )
		{
			m_tableList[ tablename ].InsertSymbol( symbol );
		}
	}

	void Environment::InsertSymbol( const Symbol& symbol )
	{
		m_pCurrentTable->InsertSymbol( symbol );
	}

	Table* Environment::GetTable( const std::string& table_name )
	{
		if ( m_tableList.find( table_name ) == m_tableList.end() )	return NULL;

		return &m_tableList[ table_name ];
	}

	Environment::Environment()
	{
		Table table( "1990" );				// ȫ�ֱ�

		m_tableList[ table.GetName() ] = table;
		m_pCurrentTable = &m_tableList[ table.GetName() ];	

		Table loader( "ETMachineEntry" );
		m_tableList[ loader.GetName() ] = loader;
		m_pCurrentTable = &m_tableList[ loader.GetName() ];
	}

	void Environment::InsertParam( const Symbol& symbol )
	{
		m_pCurrentTable->InsertParam( symbol );
	}

}