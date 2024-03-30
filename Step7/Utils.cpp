#include "stdafx.h"
#include "Utils.h"

# define M_PI 3.14159265358979323846  /* pi */

Acad::ErrorStatus createLayer(const TCHAR* sLayerName)
{
	Acad::ErrorStatus es = Acad::eOk;

	AcDbObjectId layerId = AcDbObjectId::kNull;
	AcDbLayerTablePointer pLayerTable(acdbCurDwg(), AcDb::kForRead);
	if ((es = pLayerTable->getAt(sLayerName, layerId, Adesk::kFalse)) != Acad::eOk) {
		AcDbLayerTableRecordPointer pLayerTableRec;
		pLayerTableRec.create();
		pLayerTableRec->setName(sLayerName);
		if ((es = pLayerTable->upgradeOpen()) == Acad::eOk) {
			es = pLayerTable->add(layerId, pLayerTableRec);
		}
	}
	return (es);
}

Acad::ErrorStatus createBlockRecord(const TCHAR* sName)
{
	Acad::ErrorStatus es;

	AcDbBlockTablePointer pBlockTable(acdbCurDwg(), AcDb::kForRead);
	es = pBlockTable.openStatus();
	if (es != Acad::eOk) {
		return es;
	}

	if (pBlockTable->has(sName) == Adesk::kTrue) {
		return (Acad::eDuplicateKey);
	}

	AcDbBlockTableRecordPointer pBlockTableRecord;
	pBlockTableRecord.create();
	pBlockTableRecord->setName(sName);
	pBlockTableRecord->setOrigin(AcGePoint3d::kOrigin);

	if ((es = pBlockTable->upgradeOpen()) != Acad::eOk) {
		return (es);
	}

	if ((es = pBlockTable->add(pBlockTableRecord)) != Acad::eOk) {
		return (es);
	}

	es = drawStar(pBlockTableRecord);

	return (Acad::eOk);
}

Acad::ErrorStatus drawStar(AcDbBlockTableRecord* pBlockTableRecord)
{
	Acad::ErrorStatus es = Acad::eOk;
	// Фиксированные параметры звезды
	const AcGePoint3d p3dCenter(0.0, 0.0, 0.0); // Центр звезды
	const double dRadius = 100.0; // Радиус внешних вершин
	const int nNumPoints = 5; // Количество вершин у звезды
	const double dAngle = 2 * M_PI / nNumPoints; // Угловой шаг между вершинами

	std::vector<AcGePoint3d> vp3dVerticesCoordinates;
	vp3dVerticesCoordinates.reserve(nNumPoints * 2);

	// Вычисляем координаты вершин звезды
	calculateVerticesCoordinate(p3dCenter, dRadius, nNumPoints, dAngle, vp3dVerticesCoordinates);

	// Создаём и добавляем линии для каждой стороны звезды
	es = createAndAddLineForVertices(vp3dVerticesCoordinates, pBlockTableRecord);

	return es;
}

void calculateVerticesCoordinate(const AcGePoint3d& p3dCenter, const double dRadius, const int nNumPoints, const double dAngle, std::vector<AcGePoint3d>& starPoints)
{
	for (int i = 0; i < nNumPoints * 2; ++i) {
		double r = i % 2 == 0 ? dRadius : dRadius / 2; // Уменьшенный радиус для "внутренних" вершин
		double theta = i * dAngle / 2; // Угол для каждой вершины
		double x = p3dCenter.x + r * cos(theta);
		double y = p3dCenter.y + r * sin(theta);
		starPoints.emplace_back(x, y, p3dCenter.z);
	}
}

Acad::ErrorStatus createAndAddLineForVertices(const std::vector<AcGePoint3d> vpt3dStarPoints, AcDbBlockTableRecord* pBlockTableRecord)
{
	Acad::ErrorStatus es = Acad::eOk;

	for (size_t i = 0; i < vpt3dStarPoints.size(); ++i) {
		size_t nextIndex = (i + 1) % vpt3dStarPoints.size();
		AcDbObjectPointer<AcDbLine> pLine;
		pLine.create();
		pLine->setStartPoint(vpt3dStarPoints[i]);
		pLine->setEndPoint(vpt3dStarPoints[nextIndex]);
		if (pLine == nullptr) {
			return Acad::eOutOfMemory;
		}
		pLine->setColorIndex(2);
		es = pBlockTableRecord->appendAcDbEntity(pLine);
		if (es != Acad::eOk) {
			return es;
		}
	}

	return es;
}




