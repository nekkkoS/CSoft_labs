#pragma once

#ifdef CADTEST_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"

//-----------------------------------------------------------------------------
//----- Note: Uncomment the DLLIMPEXP symbol below if you wish exporting
//----- your class to other ARX/DBX modules
class /*DLLIMPEXP*/ ChamomileBlockReactor : public AcDbObjectReactor {

public:
	ACRX_DECLARE_MEMBERS(ChamomileBlockReactor) ;

public:
	ChamomileBlockReactor () ;
	virtual ~ChamomileBlockReactor () ;

	virtual void modified(const AcDbObject* dbObj);
} ;

#ifdef CADTEST_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ChamomileBlockReactor)
#endif
