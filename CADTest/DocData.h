#pragma once
#include "DbBlockReactor.h" //TODO: заменить на stdafx???
//-----------------------------------------------------------------------------
//----- Here you can store the document / database related data.
class CDocData {

	//----- TODO: here you can add your variables

public:
	CDocData () ;
	CDocData (const CDocData &data) ;
	~CDocData () ;

	bool				m_bEditCommand;
	bool				m_bDoRepositioning;
	AcDbObjectIdArray	m_aIdChangedObjects;
	AcGePoint3dArray	m_aPt3dChamomilePositions;

	DbBlockReactor* m_pDbChamomileReactor;
} ;
