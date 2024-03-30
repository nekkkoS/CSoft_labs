#pragma once

#ifdef STEP7_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"

//-----------------------------------------------------------------------------
//----- Note: Uncomment the DLLIMPEXP symbol below if you wish exporting
//----- your class to other ARX/DBX modules
class /*DLLIMPEXP*/ ObjStarReactor : public AcDbObjectReactor {

public:
	ACRX_DECLARE_MEMBERS(ObjStarReactor) ;

public:
	ObjStarReactor () ;
	virtual ~ObjStarReactor () ;

	virtual void openedForModify(const AcDbObject* dbObj);
} ;

#ifdef STEP7_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ObjStarReactor)
#endif
