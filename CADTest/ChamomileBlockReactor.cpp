#include "StdAfx.h"
#include "ChamomileBlockReactor.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(ChamomileBlockReactor, AcDbObjectReactor, 1)

//-----------------------------------------------------------------------------
ChamomileBlockReactor::ChamomileBlockReactor () : AcDbObjectReactor() {
}

//-----------------------------------------------------------------------------
ChamomileBlockReactor::~ChamomileBlockReactor () {
}

// TODO: отрефакторить
void ChamomileBlockReactor::modified(const AcDbObject* dbObj)
{
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

    if (_tcscmp(sBlockName, _T("CHAMOMILE")) != 0)
        return;

    AcGePoint3d newPosition = pInsert->position(); // Get the position of the original block
    AcDbBlockReference* pCopy = new AcDbBlockReference(newPosition, blockId); // Create a copy of the block at the same position
    AcDbBlockTableRecordPointer pBlockTableRecordForWrite(pInsert->ownerId(), AcDb::kForWrite);
    if (pBlockTableRecordForWrite.openStatus() == Acad::eOk) {
        pBlockTableRecordForWrite->appendAcDbEntity(pCopy); // Append the copy to the block table record
        pCopy->close();
    }
    else {
        acutPrintf(_T("\nCannot open block table record for write!"));
        delete pCopy;
        return;
    }
}

