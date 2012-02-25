/*
** 作者：华亮
** 说明：
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
	** 名字：Type
	** 说明：每个符号的描述
	**
	*/
	struct SymbolType
	{
		SymbolType() : width( 0 ), structDescription( NULL ), next( NULL )	{}

		std::string type_name;			// 类型名称
		TypeKind	typeKind;			// 类型
		std::size_t	width;				// 所占字节数
		Table*		structDescription;	// 块类型的描述，指向一个符号表
		SymbolType*	next;
	};



	/*
	** 名字：TypeList
	** 说明：类型信息
	**
	*/
	class TypeList
	{
	public:
		typedef std::map< std::string, SymbolType > TypeMap;

		TypeMap			m_typeMap;
		SymbolType		m_errorType;	// 不存在的类型

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
	** 名字：AdditionInfo
	** 说明：类型附加信息
	**
	*/
	struct AdditionInfo
	{
		int reflevel;

		std::vector< int > m_vector;

		AdditionInfo() : reflevel( 0 ) {}
	};


	/*
	** 名字：Symbol
	** 说明：一个符号
	**
	*/
	class Symbol
	{
	public:
		AdditionInfo*		addInfo;	// 附加信息，数组，指针
		const SymbolType*	type;		// 符号类型
		int					offset;		// 在符号表中的偏移位置
		std::string			name;		// 符号名

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
	** 名字：Table
	** 说明： 符号表
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
	** 名字：Environment
	** 说明：总符号表
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

