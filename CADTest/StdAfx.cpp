#include "StdAfx.h"

Acad::ErrorStatus attachChamomileReactorToAllChamomile(bool attach) {
    Acad::ErrorStatus es;

    AcDbBlockTable* pBlockTable;
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    if ((es = pDb->getBlockTable(pBlockTable, AcDb::kForRead)) != Acad::eOk)
        return es;

    // Получаем запись "Model Space" для чтения
    AcDbBlockTableRecord* pModelSpace;
    if ((es = pBlockTable->getAt(ACDB_MODEL_SPACE, pModelSpace, AcDb::kForRead)) != Acad::eOk) {
        pBlockTable->close();
        return es;
    }
    pBlockTable->close();

    AcDbBlockTableRecordIterator* pIterator;
    if ((es = pModelSpace->newIterator(pIterator)) != Acad::eOk) {
        pModelSpace->close();
        return es;
    }

    // Проходим по всем записям
    for (; !pIterator->done(); pIterator->step()) {
        AcDbEntity* pEnt;
        if (pIterator->getEntity(pEnt, AcDb::kForWrite) != Acad::eOk)
            continue;

        AcDbBlockReference* pInsert = AcDbBlockReference::cast(pEnt);
        if (!pInsert) {
            pEnt->close();
            continue;
        }

        // Получаем ObjectId записи BlockTableRecord, где определена ссылка
        AcDbObjectId blockId = pInsert->blockTableRecord();
        AcDbBlockTableRecord* pBlockTableRecord;
        if (acdbOpenAcDbObject((AcDbObject*&)pBlockTableRecord, blockId, AcDb::kForRead) != Acad::eOk) {
            acutPrintf(_T("\nCannot open block table record!"));
            pEnt->close();
            break;
        }

        const TCHAR* blockName;
        pBlockTableRecord->getName(blockName);
        pBlockTableRecord->close();

        if (_tcscmp(blockName, _T("CHAMOMILE")) != 0) {
            pEnt->close();
            continue;
        }

        if (attach)
            pEnt->addReactor(pChamomileBlockReactor);
        else
            pEnt->removeReactor(pChamomileBlockReactor);

        pEnt->close();
    }

    pModelSpace->close();
    return Acad::eOk;
}

void detachAllChamomileReactors() {
    AcApDocumentIterator* pIterator = acDocManager->newAcApDocumentIterator();
    if (pIterator == nullptr)
        return;

    AcApDocument* pOldDoc = acDocManager->curDocument(); // Сохраняем текущий документ

    while (!pIterator->done()) {
        AcApDocument* pDoc = pIterator->document();
        // Если документ не заблокирован, устанавливаем его в режим записи
        if (pDoc->lockMode() == AcAp::kNone) {
            acDocManager->setCurDocument(pDoc, AcAp::kAutoWrite, Adesk::kFalse);
        }
        else {
            // Если документ заблокирован, просто устанавливаем его как текущий
            acDocManager->setCurDocument(pDoc);
        }

        attachChamomileReactorToAllChamomile(false);

        // Если документ не заблокирован, разблокируем его
        if (pDoc->lockMode() == AcAp::kNone) {
            acDocManager->unlockDocument(pDoc);
        }

        pIterator->step();
    }

    acDocManager->setCurDocument(pOldDoc, AcAp::kNone, Adesk::kFalse); // Восстанавливаем текущий документ
}
