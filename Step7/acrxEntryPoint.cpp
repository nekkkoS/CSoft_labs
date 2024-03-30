#include "StdAfx.h"
#include "resource.h"
#include "Utils.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

EdStarReactor* pEdStarReactor = nullptr;
ObjStarReactor* pObjStarReactor = nullptr;

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep7App : public AcRxArxApp {

public:
	CStep7App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
        pEdStarReactor = new EdStarReactor(true);
        pObjStarReactor = new ObjStarReactor();

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here
        detachAllEmployeeReactors();
        delete pEdStarReactor;
        delete pObjStarReactor;
		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
    static void Step7_CreateLayer() {
        try {

            AcDbLayerTablePointer pLayerTable(acdbCurDwg(), AcDb::kForWrite);
            AcDbLayerTableRecordPointer pLayerTableRecord;
            pLayerTableRecord.create();
            pLayerTableRecord->setName(L"STAR_LAYER");

            pLayerTable->add(pLayerTableRecord);
            applyCurDwgLayerTableChanges();
            acutPrintf(_T("\nLayer STAR_LAYER successfully created."));
        }
        catch (const Acad::ErrorStatus& e) {
            acutPrintf(L"\nError code: %e\n", e);
        }
    }

    static void Step7_CreateBlock() {
        if (createBlockRecord(_T("STAR")) != Acad::eOk)
            acutPrintf(_T("\nERROR: Couldn't create block record."));
        else
            acutPrintf(_T("\nBlock STAR successfully created."));
    }
	
    virtual AcRx::AppRetCode On_kLoadDwgMsg(void* pkt) {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kLoadDwgMsg(pkt);

        DocVars.docData().m_pDbStarReactor = new DbStarReactor(acdbHostApplicationServices()->workingDatabase());
        return (retCode);
    }
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep7App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep7App, Step7, _CreateLayer, CREATE_LAYER, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep7App, Step7, _CreateBlock, CREATE_BLOCK, ACRX_CMD_MODAL, NULL)