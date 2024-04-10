#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")
ChamomileBlockReactor* pChamomileBlockReactor = nullptr;
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCADTestApp : public AcRxArxApp {

public:
	CCADTestApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

        pChamomileBlockReactor = new ChamomileBlockReactor();

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		// TODO: Add your code here
        detachAllChamomileReactors();
        delete pChamomileBlockReactor;
		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here

		return (retCode);
	}

    virtual AcRx::AppRetCode On_kLoadDwgMsg(void* pkt) {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kLoadDwgMsg(pkt);

        DocVars.docData().m_pDbChamomileReactor = new DbBlockReactor(acdbHostApplicationServices()->workingDatabase());
        return (retCode);
    }

	virtual void RegisterServerComponents() {
	}

    static void CADTest_MakeChamolineBlock() {
        int nNumberPetals{};
        if (acedGetInt(_T("Enter number petals: "), &nNumberPetals) != RTNORM) {
            acutPrintf(L"Error entering number petals");
            return;
        }

        if (nNumberPetals < 2) {
            acutPrintf(L"\nNumber petals must be > 1");
            return;
        }

        Acad::ErrorStatus es;

        AcDbBlockTablePointer pBlockTable(acdbCurDwg(), AcDb::kForRead);
        es = pBlockTable.openStatus();
        if (es != Acad::eOk) {
            acutPrintf(L"\nError creating pBlockTable");
            return;
        }

        if (pBlockTable->has(L"CHAMOMILE") == Adesk::kTrue) {
            acutPrintf(L"\nBlock with CHAMOMILE name already exists");
            return;
        }

        AcDbBlockTableRecordPointer pBlockTableRecord;
        pBlockTableRecord.create();
        pBlockTableRecord->setName(L"CHAMOMILE");
        pBlockTableRecord->setOrigin(AcGePoint3d::kOrigin);
        
        if (pBlockTableRecord.openStatus() != Acad::eOk) {
            acutPrintf(L"\nError creating pBlockTableRecord");
            return;
        }

        if ((es = pBlockTable->upgradeOpen()) != Acad::eOk) {
            acutPrintf(L"\nError executing upgradeOpen");
            return;
        }

        if ((es = pBlockTable->add(pBlockTableRecord)) != Acad::eOk) {
            acutPrintf(L"\nError adding pBlockTableRecord to pBlockTable");
            return;
        }

        try {
            ChamomileDrawer chamomile(pBlockTableRecord);
            chamomile.drawChamomile(nNumberPetals);
        }
        catch (const Acad::ErrorStatus& e) {
            acutPrintf(L"\nError code: %e\n", e);
        }
        catch (...) {
            acutPrintf(L"\nError drawing chamomile");
        }
    }

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCADTestApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CCADTestApp, CADTest, _MakeChamolineBlock, MAKE_CHAMOLINE_BLOCK, ACRX_CMD_MODAL, NULL)

