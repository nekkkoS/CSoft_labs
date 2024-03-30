#include "StdAfx.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
//----- The one and only document manager object. You can use the DocVars object to retrieve
//----- document specific data throughout your application
AcApDataManager<CDocData> DocVars ;

//-----------------------------------------------------------------------------
//----- Implementation of the document data class.
CDocData::CDocData () {
	m_bEditCommand = false;
	m_bDoRepositioning = false;

	m_pDbStarReactor = nullptr;

	attachEmployeeReactorToAllEmployee(true);
}

//-----------------------------------------------------------------------------
CDocData::CDocData (const CDocData &data) {
	m_bEditCommand = false;
	m_bDoRepositioning = false;
	m_pDbStarReactor = nullptr;
}

//-----------------------------------------------------------------------------
CDocData::~CDocData () {
	if (m_pDbStarReactor) delete m_pDbStarReactor;
}
