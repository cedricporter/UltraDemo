#include "stdafx.h"
#include "FSMConfigFile.h"

int MrpFSMConfigFile::LoadFSMDescription( const Data& data )
{
	int ret = TRUE;

	m_configFileContent.clear();

	SaveStateToBuffer(data);	// ±£´æ×´Ì¬
	SavePredToBuffer(data);		// ±£´æ×´Ì¬Ìø×ªÎ½´Ê

	return ret;
}

int MrpFSMConfigFile::SaveConfigFile( const CETString& filename )
{
	int ret = FALSE;

	std::ofstream out( filename );

	if ( out.is_open() )
	{
		out << m_configFileContent;
		ret = TRUE;
	}

	return ret;
}

void MrpFSMConfigFile::SaveStateToBuffer( const Data &data ) 
{
	char tempStr[ 20 ];

	// ×´Ì¬
	m_configFileContent += "Section States\n";
	int ID = 0;
	auto m_actionList = data.GetActionList();
	std::for_each( m_actionList.begin(), m_actionList.end(), [&]( InfoUIElement* e )
	{
		e->SetID( ID++ );	// ·ÖÅäID

		m_configFileContent += "FSMState ";
		_itoa_s( e->GetID(), tempStr, 10 );
		m_configFileContent += tempStr;
		m_configFileContent += " ";
		m_configFileContent += e->GetName();
		m_configFileContent += " ";

		// Êä³ö²ÎÊý
		std::for_each( e->GetParams().begin(), e->GetParams().end(), [&]( const CETString& param )
		{ m_configFileContent += param; m_configFileContent += " "; });

		m_configFileContent += "\n";
	});
}

void MrpFSMConfigFile::SavePredToBuffer( const Data &data ) 
{
	char tempStr[ 20 ];

	// ×´Ì¬Ìø×ªÎ½´Ê
	auto m_predList = data.GetPredList();
	m_configFileContent += "\nSection Transitions\n";
	std::for_each( m_predList.begin(), m_predList.end(), [&]( PredElement* e )
	{
		m_configFileContent += "FSMTrans " ;
		_itoa_s( e->GetSource()->GetID(), tempStr, 10 );
		m_configFileContent += tempStr;
		m_configFileContent += " ";
		_itoa_s( e->GetTarget()->GetID(), tempStr, 10 );
		m_configFileContent += tempStr;
		m_configFileContent += " " ;
		m_configFileContent += e->GetName();
		m_configFileContent += " ";

		// Êä³ö²ÎÊý
		std::for_each( e->GetParams().begin(), e->GetParams().end(), [&]( const CETString& param )
		{ m_configFileContent += param; m_configFileContent += " "; });

		m_configFileContent += "\n";
	});
}