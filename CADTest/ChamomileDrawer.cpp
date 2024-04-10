#include "stdafx.h"
#include "ChamomileDrawer.h"

void ChamomileDrawer::drawChamomile(const int nNumberPetals)
{
	for (int nPetalNumber = 0; nPetalNumber < nNumberPetals; ++nPetalNumber) {
		double dAngle = calculateRotationAngle(nPetalNumber, nNumberPetals);

		AcGePoint3dArray arr3dp_PetalPoints;
		const int nPointsInSpline = 20;
		const double dInnerRadius = 250.0;
		const double dOuterRadius = 250.0;
		calculatePetalPoints(arr3dp_PetalPoints, nPointsInSpline, dInnerRadius, dOuterRadius, dAngle, nNumberPetals);

		drawPetal(arr3dp_PetalPoints);
	}

	drawFlowerCenter();
}

void ChamomileDrawer::drawPetal(const AcGePoint3dArray& arr3dp_PetalPoints)
{
	AcDbObjectPointer<AcDbHatch> pHatch = createHatch();

	AcDbObjectIdArray arrLineIds; // Массив для хранения Object ID линий контура
	drawPetalContour(arr3dp_PetalPoints, arrLineIds);

	// Выполняем заливку
	pHatch->appendLoop(AcDbHatch::kExternal, arrLineIds);
	pHatch->evaluateHatch();
	pBlockTableRecord_->appendAcDbEntity(pHatch);
}

void ChamomileDrawer::drawPetalContour(const AcGePoint3dArray& arr3dp_PetalPoints, AcDbObjectIdArray& arrLineIds)
{
	// Рисуем контур
	for (int nContourPoint = 0; nContourPoint < arr3dp_PetalPoints.length() - 1; ++nContourPoint) {
		AcDbObjectPointer<AcDbLine> pLine;
		pLine.create();
		pLine->setStartPoint(arr3dp_PetalPoints[nContourPoint]);
		pLine->setEndPoint(arr3dp_PetalPoints[nContourPoint + 1]);
		pBlockTableRecord_->appendAcDbEntity(pLine);
		arrLineIds.append(pLine->objectId());
	}

	// Замыкаем контур
	AcDbObjectPointer<AcDbLine> pClosingLine;
	pClosingLine.create();
	pClosingLine->setStartPoint(arr3dp_PetalPoints[arr3dp_PetalPoints.length() - 1]);
	pClosingLine->setEndPoint(arr3dp_PetalPoints[0]);
	pBlockTableRecord_->appendAcDbEntity(pClosingLine);
	arrLineIds.append(pClosingLine->objectId());
}

void ChamomileDrawer::drawFlowerCenter()
{
	const double dFlowerCenterRadius = 35.0;
	AcDbCircle* pFlowerCenterContour = createFlowerCenterContour(dFlowerCenterRadius);

	AcDbObjectIdArray arrIdContour;
	arrIdContour.append(pFlowerCenterContour->objectId());

	AcDbObjectPointer<AcDbHatch> pHatch = createHatch();
	pHatch->appendLoop(0, arrIdContour);
	setHatchYellowColor(pHatch);   // Устанавливаем заливке желтый цвет
	pHatch->evaluateHatch();
	pBlockTableRecord_->appendAcDbEntity(pHatch);
}

AcDbCircle* ChamomileDrawer::createFlowerCenterContour(const double dFlowerCenterRadius)
{
	// TODO: при переводе на умный указатель ломается
	AcDbCircle* pFlowerCenterContour = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, dFlowerCenterRadius);
	pBlockTableRecord_->appendAcDbEntity(pFlowerCenterContour);
	pFlowerCenterContour->close();

	return pFlowerCenterContour;
}

void ChamomileDrawer::calculatePetalPoints(AcGePoint3dArray& arr3dp_ResultPetalPoints, const int nPointsBetween,
	const double dInnerRadius, const double dOuterRadius, const double dAngle, const int nNumberPetals)
{
	for (int nCoeffT = 0; nCoeffT <= nPointsBetween; ++nCoeffT) {
		double t = static_cast<double>(nCoeffT) / nPointsBetween;
		double dCoefficient1 = 3 * (1 - t) * (1 - t) * t;
		double dcoefficient2 = 3 * (1 - t) * t * t;

		double x =
			dCoefficient1 * dInnerRadius * cos(dAngle + M_PI / nNumberPetals) +
			dcoefficient2 * dOuterRadius * cos(dAngle + 2 * M_PI / nNumberPetals);

		double y =
			dCoefficient1 * dInnerRadius * sin(dAngle + M_PI / nNumberPetals) +
			dcoefficient2 * dOuterRadius * sin(dAngle + 2 * M_PI / nNumberPetals);

		arr3dp_ResultPetalPoints.append(AcGePoint3d(x, y, 0.0));
	}
}

double ChamomileDrawer::calculateRotationAngle(const int nPetalNumber, const int nNumberPetals)
{
	double angle = (2.0 * M_PI / nNumberPetals) * nPetalNumber;
	return angle;
}

AcDbObjectPointer<AcDbHatch> ChamomileDrawer::createHatch()
{
	AcDbObjectPointer<AcDbHatch> pHatch;
	pHatch.create();
	pHatch->setPattern(AcDbHatch::kPreDefined, L"SOLID"); // Задаем тип заливки
	pHatch->setAssociative(false); // Отключаем ассоциативность заливки
	
	return pHatch;
}

void ChamomileDrawer::setHatchYellowColor(AcDbObjectPointer<AcDbHatch>& pHatch)
{
	AcCmColor flowerCenterColor;
	flowerCenterColor.setRGB(219, 170, 33);
	pHatch->setColor(flowerCenterColor);
}
