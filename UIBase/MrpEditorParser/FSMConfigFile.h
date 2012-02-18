/*
** ���ߣ�����
** ˵����
**
*/

#pragma once

#include "MrpEditorData.h"
#include <fstream>


/*
** ���֣�FSMConfigFile
** ˵������Data��������ϵ��ʵ�ִ�Data���ı���ת�����ṩ�����ļ�����
**
*/
class FSMConfigFile
{
protected:
	CETString	m_configFileContent;	// �ı������ļ������ݱ���������

public:
	virtual int LoadFSMDescription( const Data& data ) = 0;
	virtual const CETString& GetConfigFileContent() { return m_configFileContent; }
	virtual int SaveConfigFile( const CETString& filename ) = 0;
};


/*
** ���֣�MrpFSMConfigFile
** ˵�������Mrp��ʽ�������ļ�����
**
*/
class MrpFSMConfigFile : public FSMConfigFile
{
	//static const int IntialBufSize = 1000;		// m_configFileContent��ʼ����С

public:
	MrpFSMConfigFile()
	{
		//m_configFileContent.resize( IntialBufSize );
	}

	// ���ı�������Դ
	int UpdateDataFromStrContent( Data& data/*����Ӧ���Ǹ��ص�����*/ )
	{
		data.Clear();	// �������Դ�������֮ǰӦ�ñ���������Ϣ

		/**** ���ı���ͼ ****/
		
	}

	int LoadFSMDescription( const Data& data );
	int SaveConfigFile( const CETString& filename );

	/************************************************************************/
	/* ����״̬��ν�ʵ�Buffer                                                */
	/************************************************************************/
	void SavePredToBuffer( const Data &data );
	void SaveStateToBuffer( const Data &data );
};