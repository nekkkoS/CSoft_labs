#include "StdAfx.h"
#include "resource.h"
#include "../Step6_DBX/Star.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

#pragma comment (lib ,"../x64/Debug/Step6_DBX.lib")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep6_ARXApp : public AcRxArxApp {

public:
	CStep6_ARXApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	static void Step6_CreateStar() {
		int nID, nNumerPoints;
		TCHAR sColor[133];
		AcGePoint3d pt3d;
		if (acedGetInt(_T("Enter star ID: "), &nID) != RTNORM
			|| acedGetInt(_T("Enter number vertices: "), &nNumerPoints) != RTNORM
			|| acedGetString(0, _T("Enter color (red, green, blue): "), sColor) != RTNORM
			|| acedGetPoint(NULL, _T("Star position: "), asDblArray(pt3d)) != RTNORM
			) {
			return;
		}

		if (nNumerPoints < 4 || nNumerPoints > 32767) {
			acutPrintf(L"\nNumber vertices must be > 3 and < 32767");
			return;
		}

		Acad::ErrorStatus es = Acad::eOk;

		AcDbBlockTablePointer pBlockTable(acdbCurDwg(), AcDb::kForRead);
		if ((es = pBlockTable.openStatus()) != Acad::eOk) {
			acutPrintf(_T("\nCan't open pBlockTable for reading"));
			return;
		}

		AcDbBlockTableRecordPointer pSpaceRecord(ACDB_MODEL_SPACE, acdbCurDwg(), AcDb::kForWrite);
		if ((es = pSpaceRecord.openStatus()) != Acad::eOk) {
			acutPrintf(_T("\nCan't open pSpaceRecord for writing"));
			return;
		}

		AcDbObjectPointer<CStar> pEnt;
		if ((es = pEnt.create()) != Acad::eOk) {
			acutPrintf(_T("\nCan't create CStar object"));
			return;
		}

		try {
			pEnt->SetID(nID);
			pEnt->SetNumberVertices(nNumerPoints);
			pEnt->SetColor(sColor);
			pEnt->SetCenter(pt3d);
		}
		catch (...) {
			acutPrintf(_T("\nError in setters"));
			return;
		}

		AcDbObjectId idEnt;
		if ((es = pSpaceRecord->appendAcDbEntity(idEnt, pEnt)) != Acad::eOk) {
			acutPrintf(_T("\nCan't append CStar object to pSpaceRecord"));
			return;
		}
	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep6_ARXApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep6_ARXApp, Step6, _CreateStar, CREATE_STAR, ACRX_CMD_MODAL, NULL)

