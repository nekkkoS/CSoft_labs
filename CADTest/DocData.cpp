#include "StdAfx.h"

//-----------------------------------------------------------------------------
//----- The one and only document manager object. You can use the DocVars object to retrieve
//----- document specific data throughout your application
AcApDataManager<CDocData> DocVars ;

//-----------------------------------------------------------------------------
//----- Implementation of the document data class.
CDocData::CDocData () {
	m_bEditCommand = false;
	m_bDoRepositioning = false;

	m_pDbChamomileReactor = nullptr;

	attachChamomileReactorToAllChamomile(true);
}

//-----------------------------------------------------------------------------
CDocData::CDocData (const CDocData &data) {
	m_bEditCommand = false;
	m_bDoRepositioning = false;
	m_pDbChamomileReactor = nullptr;
}

//-----------------------------------------------------------------------------
CDocData::~CDocData () {
	if (m_pDbChamomileReactor) delete m_pDbChamomileReactor;
}
