#include "StdAfx.h"

// TODO: отрефакторить
Acad::ErrorStatus attachChamomileReactorToAllChamomile(bool attach)
{
	// This function looks for all block references 
	// of the block "employee" in Model Space.
	// If the insert has not been attached our object reactor, we attach it.
	Acad::ErrorStatus es;

	// Get the block table of the current database
	AcDbBlockTable* pBlockTable;
	if ((es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead)) != Acad::eOk)
		return (es);
	// Get Model Space for read.
	AcDbBlockTableRecord* pModelSpace;
	es = pBlockTable->getAt(ACDB_MODEL_SPACE, pModelSpace, AcDb::kForRead);
	pBlockTable->close();
	if (es != Acad::eOk)
		return (es);
	// Create an iterator
	AcDbBlockTableRecordIterator* pIterator;
	if ((es = pModelSpace->newIterator(pIterator)) != Acad::eOk) {
		pModelSpace->close();
		return (es);
	}
	// Go through the records
	for (; !pIterator->done(); pIterator->step()) {
		// The entity does not need to be open for write to
		// add or remove a transient reactor
		AcDbEntity* pEnt;
		es = pIterator->getEntity(pEnt, AcDb::kForRead);

		AcDbBlockReference* pInsert = AcDbBlockReference::cast(pEnt);
		if (!pInsert) {
			pEnt->close();
			continue;
		}

		// Get the ObjectId of the BlockTableRecord where the reference is defined
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

		if (_tcscmp(blockName, _T("CHAMOMILE"))) {
			pEnt->close();
			continue; // Not a chamomile
		}

		if (attach) {
			pEnt->addReactor(pChamomileBlockReactor);
		}
		else {
			pEnt->removeReactor(pChamomileBlockReactor);
		}
		pEnt->close();
	}
	delete pIterator;
	pModelSpace->close();
	return (Acad::eOk);
}

// TODO: отрефакторить
void detachAllChamomileReactors()
{
	AcApDocumentIterator* pIterator = acDocManager->newAcApDocumentIterator();
	if (pIterator == NULL)
		return;

	AcApDocument* pOldDoc = acDocManager->curDocument();

	while (!pIterator->done()) {
		AcApDocument* pDoc = pIterator->document();
		if (pDoc->lockMode() == AcAp::kNone) {
			if (acDocManager->setCurDocument(pDoc, AcAp::kAutoWrite, Adesk::kFalse) == Acad::eOk) {
				attachChamomileReactorToAllChamomile(false);
				acDocManager->unlockDocument(pDoc);
			}
		}
		else {
			acDocManager->setCurDocument(pDoc);
			attachChamomileReactorToAllChamomile(false);
		}
		pIterator->step();
	}
	delete pIterator;

	acDocManager->setCurDocument(pOldDoc, AcAp::kNone, Adesk::kFalse);
}
