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

void ChamomileBlockReactor::modified(const AcDbObject* dbObj) {
    AcDbBlockReference* pInsert = AcDbBlockReference::cast(dbObj);
    if (!pInsert) return;

    AcDbObjectId blockId = pInsert->blockTableRecord();
    AcDbObjectPointer<AcDbBlockTableRecord> pBlockTableRecord(blockId, AcDb::kForRead);
    if (pBlockTableRecord.openStatus() != Acad::eOk) {
        acutPrintf(_T("\nCannot open block table record!"));
        return;
    }

    const TCHAR* sBlockName;
    if (pBlockTableRecord->getName(sBlockName) != Acad::eOk || _tcscmp(sBlockName, _T("CHAMOMILE")) != 0) return;

    // Теперь работаем с копией блока
    AcDbObjectPointer<AcDbBlockReference> pCopy;
    if (pCopy.create() == Acad::eOk) {
        //AcGePoint3d newPosition = pInsert->position();
        //newPosition.x += 1000.0;
        pCopy->setPosition(pInsert->position());
        pCopy->setBlockTableRecord(blockId);

        AcDbBlockTableRecordPointer pBlockTableRecordForWrite(pInsert->ownerId(), AcDb::kForWrite);
        if (pBlockTableRecordForWrite.openStatus() == Acad::eOk) {
            if (pBlockTableRecordForWrite->appendAcDbEntity(pCopy) == Acad::eOk) {
                pCopy->close();
            }
            else {
                acutPrintf(_T("\nFailed to append block reference entity."));
            }
        }
        else {
            acutPrintf(_T("\nCannot open block table record for write!"));
        }
    }
    else {
        acutPrintf(_T("\nFailed to create a copy of the block reference."));
    }
}
