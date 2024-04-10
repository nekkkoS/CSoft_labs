#include "StdAfx.h"
#include "DbBlockReactor.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(DbBlockReactor, AcDbDatabaseReactor, 1)

//-----------------------------------------------------------------------------
DbBlockReactor::DbBlockReactor (AcDbDatabase *pDb) : AcDbDatabaseReactor(), mpDatabase(pDb) {
	if ( pDb )
		pDb->addReactor (this) ;
}

//-----------------------------------------------------------------------------
DbBlockReactor::~DbBlockReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void DbBlockReactor::Attach (AcDbDatabase *pDb) {
	Detach () ;
	if ( mpDatabase == NULL ) {
		if ( (mpDatabase =pDb) != NULL )
			pDb->addReactor (this) ;
	}
}

void DbBlockReactor::Detach () {
	if ( mpDatabase ) {
		mpDatabase->removeReactor (this) ;
		mpDatabase =NULL ;
	}
}

AcDbDatabase *DbBlockReactor::Subject () const {
	return (mpDatabase) ;
}

bool DbBlockReactor::IsAttached () const {
	return (mpDatabase != NULL) ;
}

// TODO: отрефакторить
void DbBlockReactor::objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
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

	if (_tcscmp(sBlockName, _T("CHAMOMILE")))
		return;

	dbObj->addReactor(pChamomileBlockReactor);

	acutPrintf(_T("\nNew object appended to the database!!!"));
}
