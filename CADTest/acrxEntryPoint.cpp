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

    static void CADTest_CreateBlock() {
        int nNumberPetals{};
        if (acedGetInt(_T("Enter number petals: "), &nNumberPetals) != RTNORM) {
            acutPrintf(L"Error entering number petals");
            return;
        }

        /*if (createBlockRecord(L"STAR", nNumberPetals) != Acad::eOk)
            acutPrintf(_T("\nERROR: Couldn't create block record."));
        else
            acutPrintf(_T("\nBlock STAR successfully created."));*/

        Acad::ErrorStatus es;

        AcDbBlockTablePointer pBlockTable(acdbCurDwg(), AcDb::kForRead);
        es = pBlockTable.openStatus();
        if (es != Acad::eOk) {
            return;
        }

        if (pBlockTable->has(L"CHAMOMILE") == Adesk::kTrue) {
            return;
        }

        AcDbBlockTableRecordPointer pBlockTableRecord;
        pBlockTableRecord.create();
        pBlockTableRecord->setName(L"CHAMOMILE");
        pBlockTableRecord->setOrigin(AcGePoint3d::kOrigin);

        if ((es = pBlockTable->upgradeOpen()) != Acad::eOk) {
            return;
        }

        if ((es = pBlockTable->add(pBlockTableRecord)) != Acad::eOk) {
            return;
        }

        ChamomileDrawer chamomile(pBlockTableRecord);
        chamomile.drawChamomile(nNumberPetals);
    }

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCADTestApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CCADTestApp, CADTest, _CreateBlock, CREATE_BLOCK, ACRX_CMD_MODAL, NULL)

