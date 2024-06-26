﻿// (C) Copyright 2002-2012 by Autodesk, Inc. 
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
//------ StdAfx.cpp : source file that includes just the standard includes
//------  StdAfx.pch will be the pre-compiled header
//------  StdAfx.obj will contain the pre-compiled type information
//-----------------------------------------------------------------------------
#include "StdAfx.h"

Acad::ErrorStatus attachEmployeeReactorToAllEmployee(bool attach)
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

		if (_tcscmp(blockName, _T("STAR"))) {
			pEnt->close();
			continue; // Not an employee
		}

		if (attach) {
			pEnt->addReactor(pObjStarReactor);
		}
		else {
			pEnt->removeReactor(pObjStarReactor);
		}
		pEnt->close();
	}
	delete pIterator;
	pModelSpace->close();
	return (Acad::eOk);
}

void detachAllEmployeeReactors()
{

	AcApDocumentIterator* pIterator = acDocManager->newAcApDocumentIterator();
	if (pIterator == NULL)
		return;

	AcApDocument* pOldDoc = acDocManager->curDocument();

	while (!pIterator->done()) {
		AcApDocument* pDoc = pIterator->document();
		if (pDoc->lockMode() == AcAp::kNone) {
			if (acDocManager->setCurDocument(pDoc, AcAp::kAutoWrite, Adesk::kFalse) == Acad::eOk) {
				attachEmployeeReactorToAllEmployee(false);
				acDocManager->unlockDocument(pDoc);
			}
		}
		else {
			acDocManager->setCurDocument(pDoc);
			attachEmployeeReactorToAllEmployee(false);
		}
		pIterator->step();
	}
	delete pIterator;

	acDocManager->setCurDocument(pOldDoc, AcAp::kNone, Adesk::kFalse);
}