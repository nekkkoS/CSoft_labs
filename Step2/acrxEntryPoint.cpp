
#include "StdAfx.h"
#include "resource.h"
#include <tchar.h>
#include <iostream>

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep2App : public AcRxArxApp {

public:
	CStep2App() : AcRxArxApp() {}

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

	static void AsdkStep2_requestPoint() {
		ads_point point;
		if (acedGetPoint(NULL, _T("Select a point: "), point) == RTNORM) {
			acutPrintf(_T("\nSelected point: (%.2f, %.2f, %.2f)"), point[0], point[1], point[2]);
		}
	}

	static void AsdkStep2_requestAngle() {
		double angle;
		if (acedGetAngle(NULL, _T("Enter an angle: "), &angle) == RTNORM) {
			acutPrintf(_T("\nEntered angle: %.2f degrees"), angle);
		}
	}

	static void AsdkStep2_selectEntity() {
		ads_name entity;
		ads_point point;

		int result = acedEntSel(_T("\nSelect entity"), entity, point);

		if (result != RTNORM)
			return;

		acutPrintf(_T("\nYou selected entity ID: %ld, %ld"), entity[0]);
	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep2App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _requestPoint, requestPoint, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _requestAngle, requestAngle, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _selectEntity, selectEntity, ACRX_CMD_TRANSPARENT, NULL);