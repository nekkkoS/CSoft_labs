#pragma once

#ifdef STEP6_DBX_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
class DLLIMPEXP CStar : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(CStar) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	CStar () ;
	virtual ~CStar () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//- Dxf Filing protocol
	virtual Acad::ErrorStatus dxfOutFields (AcDbDxfFiler *pFiler) const ;
	virtual Acad::ErrorStatus dxfInFields (AcDbDxfFiler *pFiler) ;

	//----- AcDbEntity protocols
	//- Graphics protocol

	// ----
	Acad::ErrorStatus SetCenter(const AcGePoint3d p3dCenter);
	Acad::ErrorStatus GetCenter(AcGePoint3d& p3dCenter);
	Acad::ErrorStatus SetID(const Adesk::Int32 nID);
	Acad::ErrorStatus GetID(Adesk::Int32& nID);
	Acad::ErrorStatus SetNumberVertices(const Adesk::Int32 nNumberVertices);
	Acad::ErrorStatus GetNumberVertices(Adesk::Int32& nNumberVertices);
	Acad::ErrorStatus SetColor(const TCHAR* sColor);
	Acad::ErrorStatus GetColor(TCHAR*& sColor);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;

	AcGePoint3d m_p3dCenter;
	Adesk::Int32 m_nID{};
	Adesk::Int32 m_nNumberVertices{};
	TCHAR* m_sColor{};
} ;

#ifdef STEP6_DBX_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CStar)
#endif
