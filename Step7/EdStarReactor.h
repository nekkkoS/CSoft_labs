#pragma once

#ifdef STEP7_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "aced.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
//----- Note: Uncomment the DLLIMPEXP symbol below if you wish exporting
//----- your class to other ARX/DBX modules
class /*DLLIMPEXP*/ EdStarReactor : public AcEditorReactor {

protected:
	//----- Auto initialization and release flag.
	bool mbAutoInitAndRelease ;

public:
	EdStarReactor (const bool autoInitAndRelease =true) ;
	virtual ~EdStarReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcEditor *Subject () const ;
	virtual bool IsAttached () const ;

	virtual void commandWillStart(const ACHAR* cmdStr);
	virtual void commandEnded(const ACHAR* cmdStr);

} ;

//EdStarReactor* pEdStarReactor = nullptr;