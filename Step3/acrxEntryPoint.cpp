#include "StdAfx.h"
#include "resource.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep3App : public AcRxArxApp {

public:
	CStep3App() : AcRxArxApp() {}

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

	static void AdskStep3_createLine() {
		try {
			AcGePoint3d startPt(10.0, 10.0, 0.0);
			AcGePoint3d endPt(30.0, 10.0, 0.0);

			AcDbObjectPointer<AcDbLine> pLine;
			pLine.create();
			pLine->setStartPoint(startPt);
			pLine->setEndPoint(endPt);

			AcDbBlockTablePointer pBlockTable(acdbCurDwg(), AcDb::kForRead);
			AcDbBlockTableRecordPointer pBlockTableRecord(ACDB_MODEL_SPACE, acdbCurDwg(), AcDb::kForWrite);

			AcDbObjectId lineId;
			pBlockTableRecord->appendAcDbEntity(lineId, pLine);

			acutPrintf(L"\nLine created successfully");
		}
		catch (const Acad::ErrorStatus& e) {
			acutPrintf(L"\nError code: %s\n", e);
		}
	}

	static void AsdkStep3_createCircle() {
		try {
			AcGePoint3d center(9.0, 3.0, 0.0);
			AcGeVector3d normal(0.0, 0.0, 1.0);
			double radius = 10.0;

			AcDbObjectPointer<AcDbCircle> pCirc;
			pCirc.create();
			pCirc->setCenter(center);
			pCirc->setNormal(normal);
			pCirc->setRadius(radius);

			AcDbBlockTablePointer pBlockTable(acdbCurDwg(), AcDb::kForRead);
			AcDbBlockTableRecordPointer pBlockTableRecord(ACDB_MODEL_SPACE, acdbCurDwg(), AcDb::kForWrite);

			AcDbObjectId circleId;
			pBlockTableRecord->appendAcDbEntity(circleId, pCirc);

			acutPrintf(L"\nCircle created successfully");
		}
		catch (const Acad::ErrorStatus& e) {
			acutPrintf(L"\nError code: %s\n", e);
		}
	}

	static void AsdkStep3_createNewLayer() {
		try {
			AcDbLayerTablePointer pLayerTable(acdbCurDwg(), AcDb::kForWrite);

			//TODO: непонято как перевести pLayerTableRecord на умный указатель
			AcDbLayerTableRecord* pLayerTableRecord =
				new AcDbLayerTableRecord;
			pLayerTableRecord->setName(L"NEW_LAYER");

			pLayerTable->add(pLayerTableRecord);
			pLayerTableRecord->close();
		}
		catch (const Acad::ErrorStatus& e) {
			acutPrintf(L"\nError code: %s\n", e);
		}
	}

	static void AsdkStep3_makeBlock() {
		try {
			AcDbBlockTableRecordPointer pBlockTableRec(ACDB_MODEL_SPACE, acdbCurDwg(), AcDb::kForWrite);
			pBlockTableRec.create();
			pBlockTableRec->setName(L"NEW_BLOCK");

			AcDbBlockTablePointer pBlockTable(acdbCurDwg(), AcDb::kForWrite);
			AcDbObjectId blockTableRecordId;
			pBlockTable->add(blockTableRecordId, pBlockTableRec);

			AcDbObjectPointer<AcDbLine> pLine;
			pLine.create();
			AcDbObjectId lineId;
			pLine->setStartPoint(AcGePoint3d(3, 3, 0));
			pLine->setEndPoint(AcGePoint3d(6, 6, 0));
			pLine->setColorIndex(3);
			pBlockTableRec->appendAcDbEntity(lineId, pLine);

			acutPrintf(L"\nBlock made successfully");
		}
		catch (const Acad::ErrorStatus& e) {
			acutPrintf(L"\nError code: %s\n", e);
		}
	}

	static void AsdkStep3_addLayer() {
		try {
			AcDbLayerTablePointer pLayerTbl(acdbCurDwg(), AcDb::kForWrite);
			if (!pLayerTbl->has(L"TESTLAYER")) {
				AcDbLayerTableRecord* pLayerTblRcd = new AcDbLayerTableRecord;
				pLayerTblRcd->setName(L"TESTLAYER");
				pLayerTblRcd->setIsFrozen(0);// layer to THAWED
				pLayerTblRcd->setIsOff(0);   // layer to ON
				pLayerTblRcd->setVPDFLT(0);  // viewport default
				pLayerTblRcd->setIsLocked(0);// un-locked
				AcCmColor color;
				color.setColorIndex(1); // set color to red
				pLayerTblRcd->setColor(color);
				AcDbLinetypeTablePointer pLinetypeTbl(acdbCurDwg(), AcDb::kForRead);
				AcDbObjectId ltId;
				if ((pLinetypeTbl->getAt(L"CONTINUOUS", ltId))
					!= Acad::eOk)
				{
					acutPrintf(L"\nUnable to find DASHED"
						" linetype. Using CONTINUOUS");
					pLinetypeTbl->getAt(L"CONTINUOUS", ltId);
				}
				pLayerTblRcd->setLinetypeObjectId(ltId);
				pLayerTbl->add(pLayerTblRcd);
				pLayerTblRcd->close();
			}
			else {
				acutPrintf(L"\nsuch layer already exists");
			}
		}
		catch (const Acad::ErrorStatus& e) {
			acutPrintf(L"\nError code: %s\n", e);
		}
	}

};


//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep3App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep3App, AdskStep3, _createLine, createLine, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep3App, AsdkStep3, _createCircle, createCircle, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep3App, AsdkStep3, _createNewLayer, createNewLayer, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep3App, AsdkStep3, _makeBlock, makeBlock, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep3App, AsdkStep3, _addLayer, addLayer, ACRX_CMD_TRANSPARENT, NULL);

