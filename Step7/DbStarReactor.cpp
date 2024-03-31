#include "StdAfx.h"
#include "DbStarReactor.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(DbStarReactor, AcDbDatabaseReactor, 1)

//-----------------------------------------------------------------------------
DbStarReactor::DbStarReactor (AcDbDatabase *pDb) : AcDbDatabaseReactor(), mpDatabase(pDb) {
	if ( pDb )
		pDb->addReactor (this) ;
}

//-----------------------------------------------------------------------------
DbStarReactor::~DbStarReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void DbStarReactor::Attach (AcDbDatabase *pDb) {
	Detach () ;
	if ( mpDatabase == NULL ) {
		if ( (mpDatabase =pDb) != NULL )
			pDb->addReactor (this) ;
	}
}

void DbStarReactor::Detach () {
	if ( mpDatabase ) {
		mpDatabase->removeReactor (this) ;
		mpDatabase =NULL ;
	}
}

AcDbDatabase *DbStarReactor::Subject () const {
	return (mpDatabase) ;
}

bool DbStarReactor::IsAttached () const {
	return (mpDatabase != NULL) ;
}

void DbStarReactor::objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	// TODO: непонято как перевести это на smart pointer
	//std::unique_ptr<AcDbBlockReference> pInsert = std::make_unique<AcDbBlockReference>();
	//AcDbObjectPointer<AcDbBlockReference> pInsert;
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

	dbObj->addReactor(pObjStarReactor);

	acutPrintf(_T("\nNew object appended to the database!!!"));
}
