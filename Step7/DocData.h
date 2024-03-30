#pragma once

#include "DbStarReactor.h" 


//-----------------------------------------------------------------------------
//----- Here you can store the document / database related data.
class CDocData {

	//----- TODO: here you can add your variables
public:
	bool				m_bEditCommand;
	bool				m_bDoRepositioning;
	AcDbObjectIdArray	m_aIdChangedObjects;
	AcGePoint3dArray	m_aPt3dStarPositions;

	DbStarReactor* m_pDbStarReactor;
	
public:
	CDocData () ;
	CDocData (const CDocData &data) ;
	~CDocData () ;

} ;
