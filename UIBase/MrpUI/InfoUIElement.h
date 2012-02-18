/*
** ���ߣ�����
** ˵����
**
*/
#pragma once

#include "Base.h"
#include <vector>
#include <string>

class InfoUIElement;


typedef std::function< int( InfoUIElement* ) > InfoUIElementDeletedFromDataSourceCBType;

class InfoUIElement : public UIElement
{
public:
	typedef std::vector< CETString > StringVector;

public:
	InfoUIElement() : m_ID( 0 ), m_pOnDeletedCB( NULL ) {}

	// ����������غ���
	const CETString& GetParamAt( int index ) { return m_params[ index ]; }
	const StringVector& GetParams() { return m_params; }
	int SetParamAt( int index, const CETString& param ) { m_params[ index ] = param; return TRUE; }
	void SetParams( const StringVector& params ) { m_params = params; }

	// ������������غ���
	const CETString& GetParamNameAt( int index ) { return m_paramNames[ index ]; }
	const StringVector& GetParamNames() { return m_paramNames; }
	int SetParamNameAt( int index, const CETString& param ) { m_paramNames[ index ] = param; return TRUE; }
	void SetParamNames( const StringVector& names ) { m_paramNames = names; m_params.resize(m_paramNames.size());}

	// ��������
	void SetName( const CETString& name ) { m_name = name; m_wname.assign( m_name.begin(), m_name.end() ); }
	const CETString& GetName() { return m_name; }
	const std::wstring& GetwName() { return m_wname; }

	// ����ID
	void SetID( int id ) { m_ID = id; }
	int GetID() const { return m_ID; }

	// ɾ��
	virtual int OnDeleted() { if (m_pOnDeletedCB) {return (*m_pOnDeletedCB)( this );} return 0; }
	void SetDeleteFromDataSourceCB( InfoUIElementDeletedFromDataSourceCBType* func ) 
	{ m_pOnDeletedCB = func; }

protected:
	CETString m_name;						// 
	std::wstring m_wname;					// Unicode��
	int m_ID;								// ID��State��

	StringVector m_params;					// �����б�
	StringVector m_paramNames;				// �������б�

	InfoUIElementDeletedFromDataSourceCBType* m_pOnDeletedCB;	// ������Դɾ���Ļص�����
};

