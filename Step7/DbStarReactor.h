#pragma once

#ifdef STEP7_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"
//-----------------------------------------------------------------------------
//----- Note: Uncomment the DLLIMPEXP symbol below if you wish exporting
//----- your class to other ARX/DBX modules
class /*DLLIMPEXP*/ DbStarReactor : public AcDbDatabaseReactor {

public:
	ACRX_DECLARE_MEMBERS(DbStarReactor) ;

protected:
	//----- Pointer to the document this reactor instance belongs to.
	AcDbDatabase *mpDatabase ;

public:
	DbStarReactor (AcDbDatabase *pDb =NULL) ;
	virtual ~DbStarReactor () ;

	virtual void Attach (AcDbDatabase *pDb) ;
	virtual void Detach () ;
	virtual AcDbDatabase *Subject () const ;
	virtual bool IsAttached () const ;

	virtual void objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj);

} ;

#ifdef STEP7_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DbStarReactor)
#endif
