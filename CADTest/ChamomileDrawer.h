#pragma once
#define M_PI 3.14159265358979323846

class ChamomileDrawer
{
public:
	ChamomileDrawer(AcDbBlockTableRecord* pBlockTableRecord) : pBlockTableRecord_(pBlockTableRecord) {};

	void drawChamomile(const int nNumberPetals);

private:
	void drawPetal(const AcGePoint3dArray& arr3dp_PetalPoints);

	void drawPetalContour(const AcGePoint3dArray& arr3dp_PetalPoints, AcDbObjectIdArray& arrLineIds);

	void drawFlowerCenter();

	AcDbCircle* createFlowerCenterContour(const double dFlowerCenterRadius);

	void calculatePetalPoints(AcGePoint3dArray& arr3dp_ResultPetalPoints, const int nPointsBetween,
		const double dInnerRadius, const double dOuterRadius, const double dAngle, const int nNumberPetals);

	double calculateRotationAngle(const int nPetalNumber, const int nNumberPetals);

	AcDbObjectPointer<AcDbHatch> createHatch();

	void setHatchYellowColor(AcDbObjectPointer<AcDbHatch>& pHatch);

private:
	AcDbBlockTableRecord* pBlockTableRecord_;
};

