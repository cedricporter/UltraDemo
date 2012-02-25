/*
** ���ߣ�����
** ˵����
**
*/

#pragma once


#include "def.h"



namespace ETCompiler
{


	class Table;

	template < typename T >
	inline void _assert( T b, char* message )
	{
		if ( ! b ) throw message;
	}

	enum TypeKind
	{
		CharKind,
		IntKind,
		DoubleKind,
		FloatKind,
		StructKind,
		ArrayKind,
		PointerKind
	};

	/*
	** ���֣�Type
	** ˵����ÿ�����ŵ�����
	**
	*/
	struct SymbolType
	{
		SymbolType() : width( 0 ), structDescription( NULL ), next( NULL )	{}

		std::string type_name;			// ��������
		TypeKind	typeKind;			// ����
		std::size_t	width;				// ��ռ�ֽ���
		Table*		structDescription;	// �����͵�������ָ��һ�����ű�
		SymbolType*	next;
	};



	/*
	** ���֣�TypeList
	** ˵����������Ϣ
	**
	*/
	class TypeList
	{
	public:
		typedef std::map< std::string, SymbolType > TypeMap;

		TypeMap			m_typeMap;
		SymbolType		m_errorType;	// �����ڵ�����

		TypeList()
		{
			Reserve();
		}

		void Reserve()
		{
			SymbolType type;

			type.type_name	= "int";
			type.width		= 4;
			type.typeKind	= IntKind;
			Insert( type );

			type.type_name	= "double";
			type.width		= 8;
			type.typeKind	= DoubleKind;
			Insert( type );

			type.type_name	= "float";
			type.width		= 4;
			type.typeKind	= FloatKind;
			Insert( type );

			type.type_name	= "char";	
			type.width		= 1;
			type.typeKind	= CharKind;
			Insert( type );
		}

		void Insert( const SymbolType& type )
		{
			m_typeMap[ type.type_name ] = type;
		}

		SymbolType& Lookup( const std::string& type_name )  
		{
			if ( m_typeMap.find( type_name ) == m_typeMap.end() )
			{
				return m_errorType;
			}
			return m_typeMap[ type_name ];
		}
	};


	/*
	** ���֣�AdditionInfo
	** ˵�������͸�����Ϣ
	**
	*/
	struct AdditionInfo
	{
		int reflevel;

		std::vector< int > m_vector;

		AdditionInfo() : reflevel( 0 ) {}
	};


	/*
	** ���֣�Symbol
	** ˵����һ������
	**
	*/
	class Symbol
	{
	public:
		AdditionInfo*		addInfo;	// ������Ϣ�����飬ָ��
		const SymbolType*	type;		// ��������
		int					offset;		// �ڷ��ű��е�ƫ��λ��
		std::string			name;		// ������

		Symbol() : addInfo( NULL ), type( NULL ), offset( 0 )	{}
		~Symbol()
		{
			SAFE_DELETE( addInfo );
		}

		std::size_t GetWidth() const
		{
			return type->width;
		}
	};

	
	/*
	** ���֣�Table
	** ˵���� ���ű�
	**
	*/
	class Table
	{
		typedef std::map< std::string, Symbol > SymbolList;

		//Table*			m_pPrevTable;
		SymbolList		m_paramSymbolList;
		SymbolList		m_symbolList;
		bool			m_bStructTable;
		int				m_id;
		int				m_paramTableWidth;
		int				m_tableWidth;
		std::size_t		m_symbolNum;
		std::string		m_tablename;

	public:
		Table( const std::string& name, bool bStruct = false );

		Table()
		{
			Table( "" );
		}

		void SetID( int id ) { m_id = id; }
		int GetID()	{ return m_id; }

		const SymbolList& GetLocalVarTable() { return m_symbolList; }
		const SymbolList& GetParamTable() { return m_paramSymbolList; }

		void InsertParam( const Symbol& symbol );

		void InsertSymbol( const Symbol& symbol );

		Symbol* GetSymbol( const std::string& name );

		std::size_t GetTableSize();

		std::string& GetName();
	};


	/*
	** ���֣�Environment
	** ˵�����ܷ��ű�
	**
	*/
	class Environment
	{
	public:

		std::map< std::string, Table >	m_tableList;
		TypeList						m_typeList;
		Table*							m_pCurrentTable;

		Environment();

		void AddTable( const std::string& name, bool bStruct = false );


		void InsertSymbol( const Symbol& symbol );

		void InsertParam( const Symbol& symbol );

		void InsertSymbol( const std::string tablename, const Symbol& symbol );


		void InsertType( const SymbolType& type )
		{
			m_typeList.Insert( type	);
		}

		SymbolType* GetType( const std::string type_name ) 
		{
			return &m_typeList.Lookup( type_name );
		}

		void Clear()
		{
			m_tableList.clear();
		}

		Table* GetTable( const std::string& table_name );  


	};



}

