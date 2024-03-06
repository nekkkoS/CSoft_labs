#include "StdAfx.h"
#include "resource.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep3App : public AcRxArxApp {

public:
	CStep3App () : AcRxArxApp () {}

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

	virtual void RegisterServerComponents() {
	}
	
	static void AdskStep3_createLine() {
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

	static void AsdkStep3_createCircle() {
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

	static void AsdkStep3_createNewLayer() {
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

	static void AsdkStep3_makeBlock() {
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

	static void AsdkStep3_addLayer() {
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
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep3App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep3App, AdskStep3, _createLine, createLine, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep3App, AsdkStep3, _createCircle, createCircle, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep3App, AsdkStep3, _createNewLayer, createNewLayer, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep3App, AsdkStep3, _makeBlock, makeBlock, ACRX_CMD_TRANSPARENT, NULL);
ACED_ARXCOMMAND_ENTRY_AUTO(CStep3App, AsdkStep3, _addLayer, addLayer, ACRX_CMD_TRANSPARENT, NULL);

