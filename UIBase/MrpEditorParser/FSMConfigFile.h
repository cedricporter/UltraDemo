/*
** 作者：华亮
** 说明：
**
*/

#pragma once

#include "MrpEditorData.h"
#include <fstream>


/*
** 名字：FSMConfigFile
** 说明：对Data是依赖关系，实现从Data到文本的转换和提供配置文件保存
**
*/
class FSMConfigFile
{
protected:
	CETString	m_configFileContent;	// 文本配置文件的内容保存在这里

public:
	virtual int LoadFSMDescription( const Data& data ) = 0;
	virtual const CETString& GetConfigFileContent() { return m_configFileContent; }
	virtual int SaveConfigFile( const CETString& filename ) = 0;
};


/*
** 名字：MrpFSMConfigFile
** 说明：针对Mrp格式的配置文件保存
**
*/
class MrpFSMConfigFile : public FSMConfigFile
{
	//static const int IntialBufSize = 1000;		// m_configFileContent初始化大小

public:
	MrpFSMConfigFile()
	{
		//m_configFileContent.resize( IntialBufSize );
	}

	// 从文本到数据源
	int UpdateDataFromStrContent( Data& data/*这里应该是个回调函数*/ )
	{
		data.Clear();	// 清空数据源，在清空之前应该保存坐标信息

		/**** 从文本到图 ****/
		
	}

	int LoadFSMDescription( const Data& data );
	int SaveConfigFile( const CETString& filename );

	/************************************************************************/
	/* 保存状态、谓词到Buffer                                                */
	/************************************************************************/
	void SavePredToBuffer( const Data &data );
	void SaveStateToBuffer( const Data &data );
};