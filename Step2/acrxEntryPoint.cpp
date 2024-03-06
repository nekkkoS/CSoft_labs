
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

	static void AsdkStep2_requestText() {
		TCHAR text[512];
		if (acedGetString(0, _T("\nEnter some text: "), text) == RTNORM) {
			acutPrintf(_T("\nEntered text: %s"), text);
		}
	}

	//static void AsdkStep2_createDMG() {
	//	AcDbDatabase* pDb = new AcDbDatabase();
	//	AcDbBlockTable* pBtbl;
	//	pDb->getSymbolTable(pBtbl, AcDb::kForRead);
	//	AcDbBlockTableRecord* pBtblRcd;
	//	pBtbl->getAt(ACDB_MODEL_SPACE, pBtblRcd, AcDb::kForWrite);
	//	pBtbl->close();
	//	AcDbCircle* pCir1 = new AcDbCircle(AcGePoint3d(1, 1, 1), AcGeVector3d(0, 0, 1), 1);
	//	AcDbCircle* pCir2 = new AcDbCircle(AcGePoint3d(4, 4, 4), AcGeVector3d(0, 0, 1), 2);
	//	pBtblRcd->appendAcDbEntity(pCir1);
	//	pCir1->close();
	//	pBtblRcd->appendAcDbEntity(pCir2);
	//	pCir2->close();
	//	pDb->saveAs(_T("h:\\gitCSoft\\CSoft_labs\\test.dwg"));
	//	delete pDb;
	//	// In which moment happens destroing pCir1 and pCir2?
	//}

	//static void AsdkStep2_createDMG() {
	//	std::unique_ptr<AcDbDatabase> pDb(new AcDbDatabase());
	//	AcDbBlockTable* pBtbl;
	//	pDb->getSymbolTable(pBtbl, AcDb::kForRead);
	//	AcDbBlockTableRecord* pBtblRcd;
	//	pBtbl->getAt(ACDB_MODEL_SPACE, pBtblRcd, AcDb::kForWrite);
	//	pBtbl->close();
	//	std::unique_ptr<AcDbCircle> pCir1(new AcDbCircle(AcGePoint3d(1, 1, 1), AcGeVector3d(0, 0, 1), 1));
	//	std::unique_ptr<AcDbCircle> pCir2(new AcDbCircle(AcGePoint3d(4, 4, 4), AcGeVector3d(0, 0, 1), 2));
	//	pBtblRcd->appendAcDbEntity(pCir1.get());
	//	pCir1->close();
	//	pBtblRcd->appendAcDbEntity(pCir2.get());
	//	pCir2->close();
	//	pDb->saveAs(_T("H:\\gitCSoft\\CSoft_labs"));
	//}

	//static void AsdkStep2_readDMG() {
	//	std::unique_ptr<AcDbDatabase> pDb(new AcDbDatabase(Adesk::kFalse));
	//	if (Acad::eOk != pDb->readDwgFile(_T("H:\\gitCSoft\\CSoft_labs")))
	//		return;

	//	AcDbBlockTable* pBtbl;
	//	pDb->getSymbolTable(pBtbl, AcDb::kForRead);
	//	AcDbBlockTableRecord* pBtblRcd;
	//	pBtbl->getAt(ACDB_MODEL_SPACE, pBtblRcd, AcDb::kForWrite);
	//	pBtbl->close();
	//	// How to switch it to smart pointer? and worth it?
	//	AcDbBlockTableRecordIterator* pBlkTblRcdItr;
	//	pBtblRcd->newIterator(pBlkTblRcdItr);
	//	AcDbEntity* pEnt;

	//	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step()) {
	//		pBlkTblRcdItr->getEntity(pEnt, AcDb::kForRead);
	//		acutPrintf(_T("classname: %s\n"), (pEnt->isA())->name());
	//		pEnt->close();
	//	}

	//	pBtblRcd->close();
	//	delete pBlkTblRcdItr;
	//}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep2App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _requestPoint, requestPoint, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _requestAngle, requestAngle, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _selectEntity, selectEntity, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _requestText, requestText, ACRX_CMD_TRANSPARENT, NULL);
//ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _createDMG, createDMG, ACRX_CMD_TRANSPARENT, NULL);
//ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _readDMG, readDMG, ACRX_CMD_TRANSPARENT, NULL);