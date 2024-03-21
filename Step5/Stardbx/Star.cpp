// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- Star.cpp : Implementation of Star
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Star.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
Adesk::UInt32 Star::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	Star, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, STAR,
	STARDBXAPP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------
Star::Star() : AcDbObject() {}

Star::Star(const Adesk::Int32 ID, const Adesk::Int32 NumberCorners, const TCHAR * Color)
{
	this->SetID(ID);
	this->SetNumberCorners(NumberCorners);
	this->SetColor(Color);
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus Star::dwgOutFields(AcDbDwgFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(Star::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	try {
		// Write the data members
		pFiler->writeItem(m_nID);
		pFiler->writeItem(m_nNumberCorners);
		pFiler->writeItem(m_sColor);
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError in Star::dwgInFields with Data Writing: %e\n", e);
	}

	return (pFiler->filerStatus());
}

Acad::ErrorStatus Star::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbObject::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > Star::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < Star::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	try {
		switch (version) {
		case 1:
			pFiler->readItem(&m_nID);
			pFiler->readItem(&m_nNumberCorners);
			pFiler->readItem(&m_sColor);
			break;
		}
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError in Star::dwgInFields with Data Reading: %e\n", e);
	}

	return (pFiler->filerStatus());
}

//- Dxf Filing protocol
Acad::ErrorStatus Star::dxfOutFields(AcDbDxfFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbObject::dxfOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	es = pFiler->writeItem(AcDb::kDxfSubclass, _RXST("Star"));
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(kDxfInt32, Star::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....
	try {
		// Write out the data members
		pFiler->writeItem(AcDb::kDxfInt32, m_nID);
		pFiler->writeItem(AcDb::kDxfInt32 + 1, m_nNumberCorners);
		pFiler->writeItem(AcDb::kDxfXTextString, m_sColor);
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError in Star::dxfOutFields with Data Writing: %e\n", e);
	}

	return (pFiler->filerStatus());
}

Acad::ErrorStatus Star::dxfInFields(AcDbDxfFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbObject::dxfInFields(pFiler);
	if (es != Acad::eOk || !pFiler->atSubclassData(_RXST("Star")))
		return (pFiler->filerStatus());
	//----- Object version number needs to be read first
	struct resbuf rb;
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt32) {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32);
		return (pFiler->filerStatus());
	}
	Adesk::UInt32 version = (Adesk::UInt32)rb.resval.rlong;
	if (version > Star::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < Star::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while (es == Acad::eOk && (es = pFiler->readResBuf(&rb)) == Acad::eOk) {
		switch (rb.restype) {
		case AcDb::kDxfInt32:
			m_nID = rb.resval.rlong;
			break;
		case AcDb::kDxfInt32 + 1:
			m_nNumberCorners = rb.resval.rlong;
			break;
		case AcDb::kDxfXTextString:
			if (m_sColor != NULL)
				free(m_sColor);
			m_sColor = _tcsdup(rb.resval.rstring);
			break;

		default:
			//----- An unrecognized group. Push it back so that the subclass can read it again.
			pFiler->pushBackItem();
			es = Acad::eEndOfFile;
			break;
		}
	}
	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if (es != Acad::eEndOfFile)
		return (Acad::eInvalidResBuf);

	return (pFiler->filerStatus());
}

Acad::ErrorStatus Star::SetID(const Adesk::Int32 ID)
{
	assertWriteEnabled();
	m_nID = ID;

	return Acad::eOk;;
}

Acad::ErrorStatus Star::GetID(Adesk::Int32 & ID)
{
	assertReadEnabled();
	ID = m_nID;

	return Acad::eOk;
}

Acad::ErrorStatus Star::SetNumberCorners(const Adesk::Int32 NumberCorners)
{
	assertWriteEnabled();
	m_nNumberCorners = NumberCorners;

	return Acad::eOk;
}

Acad::ErrorStatus Star::GetNumberCorners(Adesk::Int32 & NumberCorners)
{
	assertReadEnabled();
	NumberCorners = m_nNumberCorners;

	return Acad::eOk;
}

Acad::ErrorStatus Star::SetColor(const TCHAR * Color)
{
	assertWriteEnabled();
	if (m_sColor) {
		free(m_sColor);
	}
	m_sColor = _tcsdup(Color);

	return Acad::eOk;
}

Acad::ErrorStatus Star::GetColor(TCHAR * &Color)
{
	assertReadEnabled();
	Color = m_sColor;

	return Acad::eOk;
}

