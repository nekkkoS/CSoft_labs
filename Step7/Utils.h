#pragma once

Acad::ErrorStatus createLayer(const TCHAR* sLayerName);
Acad::ErrorStatus createBlockRecord(const TCHAR* sName);

// Можно, конечно, вынести звезду в отдельный класс для соблюдения инкапсуляции, но, думаю, в рамках этой лабы смысла в этом немного
Acad::ErrorStatus drawStar(AcDbBlockTableRecord* pBlockTableRecord);
void calculateVerticesCoordinate(const AcGePoint3d& p3dCenter, const double dRadius, const int nNumPoints, const double dAngle, std::vector<AcGePoint3d>& starPoints);
Acad::ErrorStatus createAndAddLineForVertices(const std::vector<AcGePoint3d> vpt3dStarPoints, AcDbBlockTableRecord* pBlockTableRecord);