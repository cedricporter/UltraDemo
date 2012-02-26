#include "stdafx.h"
#include "ETController.h"
#include "def.h"
#include "JiafeiDebugger.h"
#include "Parser.h"
#include "ETMachine.h"
#include <fstream>
#include <string>
#include "MessageDefinition.h"
#include <algorithm>


namespace ETCompiler
{

	//////////////////////////////////////////////////////////////////////////
	//
	// Class MemoryViewDebugger
	// 
	// 罗嘉飞  2011、5、26  内存显示
	//
	void MemoryViewDebugger::Debug( VCPU* pCPU, LPVOID param )
	{
		auto IR = pCPU->GetIR();
		if ( IR->m_elements[ 0 ] == "leave" )  {flag = 0; m_funcId.pop();}
		else if ( IR->m_elements[ 0 ] == "mov" && IR->m_elements[ 1 ] == "d1" ) {flag = 1;m_funcId.push( pCPU->Getdreg()[ 1 ] );}

		if ( !m_funcId.empty() ) 
		{
			if ( m_funcId.top()  !=  pCPU->Getdreg()[ 1 ] ) 
				flag = 1;
		}
		if ( flag == 1 )
		{
			// 获取语法器
			IParser* pParser = (IParser*)m_callBack( NULL );
			// 获取符号表
			Environment& env = (Environment)pParser->GetTables();

            ETController* controller = (ETController*)m_callBack((LPVOID)1);

			dword_pointer pebp = pCPU->Getebp();
			dword_pointer pesp = pCPU->Getesp(); 
			dword eip = pCPU->Geteip();

			PropertiesInsertItem temp;
			WatchPropertiesListType* ValList = &controller->m_memoryList;
            ValList->clear();


			//////////////////////
			DWORD pc = pCPU->Geteip();	// PC
			dword func_id = m_funcId.top();    //每次都去栈顶 
			Table* table;

			auto iter=env.m_tableList.begin(); 
			for (;iter!=env.m_tableList.end();iter++) 
			{
				if( iter->second.GetID()==func_id)
				{
					break;
				}
			}

			if ( iter == env.m_tableList.end() )	return;

			table = &iter->second;
			/*(table)->m_symbolList;*/

			for ( auto symbol = table->GetLocalVarTable().begin(); symbol != table->GetLocalVarTable().end(); ++symbol )
			{
				strcpy_s(temp.varname, symbol->second.name.c_str());
				temp.val=(dword)(dword_pointer)( ((uchar_pointer)pebp) - symbol->second.offset);
				ValList->push_back(temp);
			}

			/*table->m_paramSymbolList;*/
			for ( auto symbol = table->GetParamTable().begin(); symbol != table->GetParamTable().end(); ++symbol )
			{
				strcpy_s(temp.varname, symbol->second.name.c_str());
				temp.val=(dword)(dword_pointer)( ((uchar_pointer)pebp) - symbol->second.offset + 4);
				ValList->push_back(temp);
			}

			//if ( listComp != *ValList )
			//{

			//	listComp = *ValList;
			//	//pProWnd->PostMessageA( WM_CLEAR_ALL_ITEMS );
			//	//pProWnd->PostMessageA( WM_INSERT_ITEM, (WPARAM)ValList, 0 );
			//}
			//else
			//{
			//	delete ValList;
			//}
			
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// Class WatchDogDebugger
	// 	
	// 罗嘉飞   2011/5/26   需要看到的内存
	//
	void WatchDogDebugger::Debug( VCPU* pCPU, LPVOID param )
	{
		auto IR = pCPU->GetIR();
		if ( IR->m_elements[ 0 ] == "leave" )  {flag = 0; m_funcId.pop();}
		else if ( IR->m_elements[ 0 ] == "mov" && IR->m_elements[ 1 ] == "d1" ) {flag = 1;m_funcId.push( pCPU->Getdreg()[ 1 ] );}

		if ( !m_funcId.empty() ) 
		{
			if ( m_funcId.top()  !=  pCPU->Getdreg()[ 1 ] ) 
				flag = 1;
		}

		if( flag==1)  
		{
			// 获取语法器
			IParser* pParser = (IParser*)m_callBack( 0 );
			// 获取符号表
			Environment& env = (Environment)pParser->GetTables();

			dword_pointer pebp = pCPU->Getebp();

			dword_pointer pesp = pCPU->Getesp();

			dword eip = pCPU->Geteip();

			std::list<std::string>* pList = (std::list<std::string>*)m_callBack( (LPVOID)1 );

			PropertiesInsertItem temp;
			std::list< PropertiesInsertItem >* ValList = new std::list< PropertiesInsertItem >;

			//////////////////////
			DWORD pc = pCPU->Geteip();	// PC

			dword func_id = m_funcId.top();

			Table* table;

			auto iter=env.m_tableList.begin();

			for (;iter!=env.m_tableList.end();iter++) 
			{
				if( iter->second.GetID()==func_id)
				{
					break;
				}
			}

			if ( iter == env.m_tableList.end() )	return;

			table = &iter->second;


			for ( auto symbol = table->GetLocalVarTable().begin(); symbol != table->GetLocalVarTable().end(); ++symbol )
			{
				//在关键字List里才取出
				for (auto iter = pList->begin(); iter != pList->end(); ++iter)
				{
					if ( *iter == symbol->second.name)
					{
						strcpy_s(temp.varname, symbol->second.name.c_str());
						temp.val=(dword)(dword_pointer)( ((uchar_pointer)pebp) - symbol->second.offset);
						ValList->push_back(temp);
					}
				}
			}


			for ( auto symbol = table->GetParamTable().begin(); symbol != table->GetParamTable().end(); ++symbol )
			{
				//在关键字List里才取出
				for (auto iter = pList->begin(); iter != pList->end(); ++iter)
				{
					if ( *iter == symbol->second.name)
					{
						strcpy_s(temp.varname, symbol->second.name.c_str());
						temp.val=(dword)(dword_pointer)( ((uchar_pointer)pebp) - symbol->second.offset+4);
						ValList->push_back(temp);
					}
				}
			}

			m_callBack( (LPVOID)ValList );

	/*		CPropertiesWnd* pProWnd = *(CPropertiesWnd**)m_callBack( (LPVOID)2 );
			if ( listComp != *ValList )
			{

				listComp = *ValList;
				pProWnd->PostMessageA( WM_CLEAR_ALL_ITEMS );
				pProWnd->PostMessageA( WM_INSERT_ITEM, (WPARAM)ValList, 0 );

			}*/


			//static int i = 0;

			//if ( ! ValList->empty() )
			//{
			//	std::ofstream out("d:\\data\\" + itoa( i++ ) + ".txt",std::ios::out);
			//	out << eip << std::endl;
			//	for (auto iter = ValList->begin(); iter!=ValList->end(); ++iter)
			//	{
			//		out << iter->varname << "  ";
			//		out << iter->val << std::endl;
			//	}
			//}
	}
	}

}
	