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
	CStep6_ARXApp () : AcRxArxApp () {}

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
	
	static void Step6_CreateStar () {
		int nID, nNumerPoints;
		TCHAR sColor[133];
		AcGePoint3d pt;
		if (acedGetInt(_T("Enter star ID: "), &nID) != RTNORM
			|| acedGetInt(_T("Enter number vertices: "), &nNumerPoints) != RTNORM
			|| acedGetString(0, _T("Enter color (Red, Green, Blue): "), sColor) != RTNORM
			|| acedGetPoint(NULL, _T("Star position: "), asDblArray(pt)) != RTNORM
			) {
			return;
		}

		// Get a pointer to the current drawing
		// and get the drawing's block table. Open it for read.
		AcDbBlockTable* pBlockTable;
		if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead) == Acad::eOk) {
			// Get the Model Space record and open it for write. This will be the owner of the new employee entity.
			AcDbBlockTableRecord* pSpaceRecord;
			if (pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite) == Acad::eOk) {
				
				CStar* pEnt = new CStar;
				pEnt->SetID(nID);
				pEnt->SetNumberVertices(nNumerPoints);
				pEnt->SetColor(sColor);
				pEnt->SetCenter(pt);
				// Append pEnt to Model Space, then close it and the Model Space record.
				AcDbObjectId idObj;
				if (pSpaceRecord->appendAcDbEntity(idObj, pEnt) == Acad::eOk)
					pEnt->close();
				else
					delete pEnt;
				pSpaceRecord->close();
			}
			pBlockTable->close();
		}
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep6_ARXApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep6_ARXApp, Step6, _CreateStar, CREATE_STAR, ACRX_CMD_MODAL, NULL)

