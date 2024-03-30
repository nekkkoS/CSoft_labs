#include "StdAfx.h"
#include "ObjStarReactor.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(ObjStarReactor, AcDbObjectReactor, 1)

//-----------------------------------------------------------------------------
ObjStarReactor::ObjStarReactor () : AcDbObjectReactor() {
}

//-----------------------------------------------------------------------------
ObjStarReactor::~ObjStarReactor () {
}

void ObjStarReactor::openedForModify(const AcDbObject* dbObj)
{
	if (DocVars.docData().m_bDoRepositioning)
		return;

	// If none of our monitored commands is active return
	if (!DocVars.docData().m_bEditCommand)
		return;

	// TODO: не понял как перевести это на smart pointer
	AcDbBlockReference* pInsert = AcDbBlockReference::cast(dbObj);
	if (!pInsert)
		return;

	AcDbObjectId blockId = pInsert->blockTableRecord();
	AcDbObjectPointer<AcDbBlockTableRecord> pBlockTableRecord(blockId, AcDb::kForRead);
	if (pBlockTableRecord.openStatus() != Acad::eOk) {
		acutPrintf(_T("\nCannot open block table record!"));
		return;
	}

	const TCHAR* sBlockName;
	pBlockTableRecord->getName(sBlockName);
	pBlockTableRecord->close();

	if (_tcscmp(sBlockName, _T("STAR")))
		return;

	DocVars.docData().m_aIdChangedObjects.append(pInsert->objectId());
	DocVars.docData().m_aPt3dStarPositions.append(pInsert->position());

	acutPrintf(L"\n---------ObjectReactor!!!!!!!!!!");
}

