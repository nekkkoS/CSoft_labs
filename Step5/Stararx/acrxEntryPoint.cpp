#include "StdAfx.h"
#include "resource.h"
#include "Utils.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")
# define M_PI           3.14159265358979323846  /* pi */
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStararxApp : public AcRxArxApp {

public:
	CStararxApp () : AcRxArxApp () {}

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
	
	static void Step5_CreateLayer () {
		try {
			AcDbLayerTablePointer pLayerTable(acdbCurDwg(), AcDb::kForWrite);

			AcDbLayerTableRecordPointer pLayerTableRecord;
			pLayerTableRecord.create();

			const TCHAR* sLayerName = L"STAR_LAYER";
			pLayerTableRecord->setName(sLayerName);

			pLayerTable->add(pLayerTableRecord);
			applyCurDwgLayerTableChanges();
			acutPrintf(L"Layer '%s' created successfully", sLayerName);
		}
		catch (const Acad::ErrorStatus& e) {
			acutPrintf(L"\nError in Step5_CreateLayer: %e\n", e);
		}
	}

	static void Step5_CreateStarBlock() {
		try {
			// Создание записи блока для нового блока
			AcDbBlockTableRecordPointer pBlockTableRec(ACDB_MODEL_SPACE, acdbCurDwg(), AcDb::kForWrite);
			pBlockTableRec.create();
			const TCHAR* sBlockName = L"Star_Block";
			pBlockTableRec->setName(sBlockName);

			// Добавление записи блока в таблицу блоков
			AcDbBlockTablePointer pBlockTable(acdbCurDwg(), AcDb::kForWrite);
			AcDbObjectId objId_blockTableRecordId;
			pBlockTable->add(objId_blockTableRecordId, pBlockTableRec);

			// Фиксированные параметры звезды
			AcGePoint3d pt3d_Center(0.0, 0.0, 0.0); // Центр звезды
			double d_radius = 1.0; // Радиус внешних вершин
			const int i_numPoints = 5; // Количество вершин у звезды
			double d_angle = 2 * M_PI / i_numPoints; // Угловой шаг между вершинами

			// Вычисляем координаты вершин звезды
			std::vector<AcGePoint3d> vpt3d_starPoints;
			for (int i = 0; i < i_numPoints * 2; ++i) {
				double r = i % 2 == 0 ? d_radius : d_radius / 2; // Уменьшенный радиус для "внутренних" вершин
				double theta = i * d_angle / 2; // Угол для каждой вершины
				double x = pt3d_Center.x + r * cos(theta);
				double y = pt3d_Center.y + r * sin(theta);
				vpt3d_starPoints.push_back(AcGePoint3d(x, y, pt3d_Center.z));
			}

			// Создаём и добавляем линии для каждой стороны звезды
			for (size_t i = 0; i < vpt3d_starPoints.size(); ++i) {
				size_t n_NextIndex = (i + 1) % vpt3d_starPoints.size(); // Следующая вершина для соединения
				AcDbObjectPointer<AcDbLine> pLine;
				pLine.create();
				pLine->setStartPoint(vpt3d_starPoints[i]);
				pLine->setEndPoint(vpt3d_starPoints[n_NextIndex]);
				pLine->setColorIndex(2); // Устанавливаем желтый цвет
				// Добавляем линию в запись блока
				AcDbObjectId objId_LineId;
				pBlockTableRec->appendAcDbEntity(objId_LineId, pLine);
			}

			acutPrintf(L"\n'%s' made successfully", sBlockName);
		}
		catch (const Acad::ErrorStatus& e) {
			acutPrintf(L"\nError in Step5_CreateStarBlock: %e\n", e);
		}
	}

	static void Step5_SetLayer() {
		try {
			const TCHAR* sLayerName = L"Star_Layer";
			const TCHAR* sBlockName = L"Star_Block";

			// 1 Open the block table of the current working database
			AcDbBlockTablePointer pBTable(acdbCurDwg(), AcDb::kForRead);

			// 2 Get the MODEL_SPACE block table record
			AcDbBlockTableRecord* pBTRecord;
			pBTable->getAt(ACDB_MODEL_SPACE, pBTRecord, AcDb::kForRead);

			// 3 Obtain a block table record iterator
			AcDbBlockTableRecordIterator* pBTRIterator;
			pBTRecord->newIterator(pBTRIterator, Adesk::kTrue, Adesk::kTrue);
			pBTRecord->close();

			// 4 Itrerate through the MODEL_SPACE
			TCHAR* s_CurrentBlockName;
			AcDbEntity* pEntity;
			AcDbObjectId objId_BlockId;

			for (pBTRIterator->start(); !pBTRIterator->done(); pBTRIterator->step())
			{
				// 5 Open object for read
				if (pBTRIterator->getEntity(pEntity, AcDb::kForRead, Adesk::kFalse) != Acad::eOk)
				{
					acedAlert(L"\nCan't open object.");
					continue;
				}

				// 6 Check if the entity is a block reference
				if (pEntity->isA() != AcDbBlockReference::desc())
				{
					pEntity->close();
					continue;
				}

				// 7 Obtain the block table record of the reference and check it name
				objId_BlockId = (AcDbBlockReference::cast(pEntity))->blockTableRecord();

				if (acdbOpenObject(pBTRecord, objId_BlockId, AcDb::kForRead) == Acad::eOk)
				{
					pBTRecord->getName(s_CurrentBlockName);
					if (_tcscmp(s_CurrentBlockName, sBlockName) == 0)
					{
						if (pEntity->upgradeOpen() == Acad::eOk)
							pEntity->setLayer(sLayerName);
					}
					pBTRecord->close();
					acdbFree(s_CurrentBlockName);
				}
				pEntity->close();
			}
			acutPrintf(_T("\nSet Layer \"%s\" for \"%s\"."), sLayerName, sBlockName);
		}
		catch (const Acad::ErrorStatus& e) {
			acutPrintf(L"\nError in Step5_SetLayer: %e\n", e);
		}
	}

	static void Step5_AddDetail() {
		AddDetails(L"STARS_DICTIONARY", L"DETAILS");
	}

	static void Step5_RemoveDetail() {
		RemoveDetails(L"STARS_DICTIONARY", L"DETAILS");
	}

	static void Step5_ListDetails() {
		ListDetails(L"STARS_DICTIONARY", L"DETAILS");
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStararxApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CStararxApp, Step5, _CreateLayer, Create_Layer, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStararxApp, Step5, _CreateStarBlock, Create_Star_Block, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStararxApp, Step5, _SetLayer, Set_Layer, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStararxApp, Step5, _AddDetail, Add_Detail, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStararxApp, Step5, _RemoveDetail, Remove_Detail, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStararxApp, Step5, _ListDetails, List_Details, ACRX_CMD_MODAL, NULL)

