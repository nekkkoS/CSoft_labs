
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

	static void AsdkStep2_createLine() {
		AcGePoint3d startPt(10.0, 10.0, 0.0);
		AcGePoint3d endPt(30.0, 10.0, 0.0);

		// Pointer to line object
		AcDbLine* pLine = new AcDbLine(startPt, endPt);

		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pBlockTable, AcDb::kForRead);

		AcDbBlockTableRecord* pBlockTableRecord;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
			AcDb::kForWrite);

		pBlockTable->close();

		AcDbObjectId lineId;
		pBlockTableRecord->appendAcDbEntity(lineId, pLine);
		pBlockTableRecord->close();
		pLine->close();
	}

	static void AsdkStep2_createCircle() {
		AcGePoint3d center(9.0, 3.0, 0.0);
		AcGeVector3d normal(0.0, 0.0, 1.0);
		double radius = 10.0;
		AcDbCircle* pCirc = new AcDbCircle(center, normal, radius);

		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pBlockTable, AcDb::kForRead);

		AcDbBlockTableRecord* pBlockTableRecord;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
			AcDb::kForWrite);
		pBlockTable->close();
		AcDbObjectId circleId;
		pBlockTableRecord->appendAcDbEntity(circleId, pCirc);
		pBlockTableRecord->close();
		pCirc->close();
	}

	static void AsdkStep2_createNewLayer() {
		AcDbLayerTable* pLayerTable;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pLayerTable, AcDb::kForWrite);
		AcDbLayerTableRecord* pLayerTableRecord =
			new AcDbLayerTableRecord;
		pLayerTableRecord->setName(L"NEW_LAYER");
		// Defaults are used for other properties of 
		// the layer if they are not otherwise specified.
		//
		pLayerTable->add(pLayerTableRecord);
		pLayerTable->close();
		pLayerTableRecord->close();
	}

	static void AsdkStep2_makeBlock() {
		// Create and name a new block table record.
		//
		AcDbBlockTableRecord* pBlockTableRec
			= new AcDbBlockTableRecord();
		pBlockTableRec->setName(L"NEW_BLOCK");
		// Get the block table.
		//
		AcDbBlockTable* pBlockTable = NULL;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pBlockTable, AcDb::kForWrite);
		// Add the new block table record to the block table.
		//
		AcDbObjectId blockTableRecordId;
		pBlockTable->add(blockTableRecordId, pBlockTableRec);
		pBlockTable->close();
		// Create and add a line entity to the component's
		// block record.
		//
		AcDbLine* pLine = new AcDbLine();
		AcDbObjectId lineId;
		pLine->setStartPoint(AcGePoint3d(3, 3, 0));
		pLine->setEndPoint(AcGePoint3d(6, 6, 0));
		pLine->setColorIndex(3);
		pBlockTableRec->appendAcDbEntity(lineId, pLine);
		pLine->close();
		pBlockTableRec->close();
	}

	static void AsdkStep2_printAll()
	{
		int rc;
		ACHAR blkName[50];
		rc = acedGetString(Adesk::kTrue,
			_T("Enter Block Name <CR for current space>: "),
			blkName);
		if (rc != RTNORM)
			return;
		if (blkName[0] == '\0') {
			if (acdbHostApplicationServices()->workingDatabase()
				->tilemode() == Adesk::kFalse) {
				struct resbuf rb;
				acedGetVar(L"cvport", &rb);
				if (rb.resval.rint == 1) {
					_tcscpy(blkName, ACDB_PAPER_SPACE);
				}
				else {
					_tcscpy(blkName, ACDB_MODEL_SPACE);
				}
			}
			else {
				_tcscpy(blkName, ACDB_MODEL_SPACE);
			}
		}
		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord* pBlockTableRecord;
		pBlockTable->getAt(blkName, pBlockTableRecord,
			AcDb::kForRead);
		pBlockTable->close();
		AcDbBlockTableRecordIterator* pBlockIterator;
		pBlockTableRecord->newIterator(pBlockIterator);
		for (; !pBlockIterator->done();
			pBlockIterator->step())
		{
			AcDbEntity* pEntity;
			pBlockIterator->getEntity(pEntity, AcDb::kForRead);
			AcDbHandle objHandle;
			pEntity->getAcDbHandle(objHandle);
			TCHAR handleStr[20];
			objHandle.getIntoAsciiBuffer(handleStr);
			const ACHAR* pCname = pEntity->isA()->name();
			acutPrintf(L"Object Id %lx, handle %s, class %s.\n",
				pEntity->objectId(), handleStr, pCname);
			pEntity->close();
		}
		delete pBlockIterator;
		pBlockTableRecord->close();
		acutPrintf(L"\n");
	}

	static void AsdkStep2_addLayer() {
		AcDbLayerTable* pLayerTbl;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pLayerTbl, AcDb::kForWrite);
		if (!pLayerTbl->has(L"ASDK_TESTLAYER")) {
			AcDbLayerTableRecord* pLayerTblRcd
				= new AcDbLayerTableRecord;
			pLayerTblRcd->setName(L"ASDK_TESTLAYER");
			pLayerTblRcd->setIsFrozen(0);// layer to THAWED
			pLayerTblRcd->setIsOff(0);   // layer to ON
			pLayerTblRcd->setVPDFLT(0);  // viewport default
			pLayerTblRcd->setIsLocked(0);// un-locked
			AcCmColor color;
			color.setColorIndex(1); // set color to red
			pLayerTblRcd->setColor(color);
			// For linetype, we need to provide the object ID of
			// the linetype record for the linetype we want to
			// use. First, we need to get the object ID.
			//
			AcDbLinetypeTable* pLinetypeTbl;
			AcDbObjectId ltId;
			acdbHostApplicationServices()->workingDatabase()
				->getSymbolTable(pLinetypeTbl, AcDb::kForRead);
			if ((pLinetypeTbl->getAt(L"DASHED", ltId))
				!= Acad::eOk)
			{
				acutPrintf(L"\nUnable to find DASHED"
					" linetype. Using CONTINUOUS");
				// CONTINUOUS is in every drawing, so use it.
				//
				pLinetypeTbl->getAt(L"CONTINUOUS", ltId);
			}
			pLinetypeTbl->close();
			pLayerTblRcd->setLinetypeObjectId(ltId);
			pLayerTbl->add(pLayerTblRcd);
			pLayerTblRcd->close();
			pLayerTbl->close();
		}
		else {
			pLayerTbl->close();
			acutPrintf(L"\nlayer already exists");
		}
	}

	static void AsdkStep2_iterateLinetypes() {
		AcDbLinetypeTable* pLinetypeTbl;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pLinetypeTbl, AcDb::kForRead);
		// Create a new iterator that starts at table
		// beginning and skips deleted.
		//
		AcDbLinetypeTableIterator* pLtIterator;
		pLinetypeTbl->newIterator(pLtIterator);
		// Walk the table, getting every table record and
		// printing the linetype name.
		//
		AcDbLinetypeTableRecord* pLtTableRcd;
		ACHAR* pLtName;
		for (; !pLtIterator->done(); pLtIterator->step()) {
			pLtIterator->getRecord(pLtTableRcd, AcDb::kForRead);
			pLtTableRcd->getName(pLtName);
			pLtTableRcd->close();
			acutPrintf(L"\nLinetype name is:  %s", pLtName);
			free(pLtName);
		}
		delete pLtIterator;
		pLinetypeTbl->close();
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
	//	// А в какой момент уничтожаются pCir1 и pCir2?
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
	//	// А как? и стоит ли на уммный указатель перевести?
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
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _createLine, createLine, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _createCircle, createCircle, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _createNewLayer, createNewLayer, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _makeBlock, makeBlock, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _printAll, printAll, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _addLayer, addLayer, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _iterateLinetypes, iterateLinetypes, ACRX_CMD_TRANSPARENT, NULL);
//ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _createDMG, createDMG, ACRX_CMD_TRANSPARENT, NULL);
//ACED_ARXCOMMAND_ENTRY_AUTO(CStep2App, AsdkStep2, _readDMG, readDMG, ACRX_CMD_TRANSPARENT, NULL);