#include "StdAfx.h"
#include "resource.h"
#include "aced.h"
#include "dbsymtb.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep1App : public AcRxArxApp {

public:
	CStep1App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static void AdskStep1_HelloWorld () {
		acutPrintf(_T("\nHello, World!"));
	}

	static void HelloWorld() {
		acutPrintf(_T("\nHello, World!"));
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep1App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep1App, AdskStep1, _HelloWorld, HelloWorld, ACRX_CMD_MODAL, NULL)

