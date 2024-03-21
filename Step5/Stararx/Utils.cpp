#include "stdafx.h"
#include "Utils.h"
#include <stdexcept>

# define M_PI 3.14159265358979323846  /* pi */

void GetRefObject(AcDbObject*& pObject, AcDb::OpenMode mode) {
	ads_name entytyName;
	ads_point entityPoint;


	if (acedEntSel(L"Select star: ", entytyName, entityPoint) != RTNORM) {
		pObject = nullptr;
		return;
	}

	AcDbObjectId objectID;
	if (acdbGetObjectId(objectID, entytyName) != Acad::eOk) {
		pObject = nullptr;
		return;
	}

	if (acdbOpenAcDbObject(pObject, objectID, mode) != Acad::eOk) {
		pObject = nullptr;
		return;
	}

	if (!pObject->isKindOf(AcDbBlockReference::desc())) {
		pObject->close();
		pObject = nullptr;
		return;
	}

	return;
}

void AddStarBlock(AcDbBlockTableRecord* pBlockTable)
{
	// Фиксированные параметры звезды
	AcGePoint3d center(0.0, 0.0, 0.0); // Центр звезды
	double radius = 1.0; // Радиус внешних вершин
	const int numPoints = 5; // Количество вершин у звезды
	double angle = 2 * M_PI / numPoints; // Угловой шаг между вершинами

	// Вычисляем координаты вершин звезды
	std::vector<AcGePoint3d> starPoints;
	for (int i = 0; i < numPoints * 2; ++i) {
		double r = i % 2 == 0 ? radius : radius / 2; // Уменьшенный радиус для "внутренних" вершин
		double theta = i * angle / 2; // Угол для каждой вершины
		double x = center.x + r * cos(theta);
		double y = center.y + r * sin(theta);
		starPoints.push_back(AcGePoint3d(x, y, center.z));
	}

	// Создаём и добавляем линии для каждой стороны звезды
	for (size_t i = 0; i < starPoints.size(); ++i) {
		size_t nextIndex = (i + 1) % starPoints.size(); // Следующая вершина для соединения
		AcDbLine* pLine = new AcDbLine(starPoints[i], starPoints[nextIndex]);
		pLine->setColorIndex(2); // Устанавливаем желтый цвет
		pBlockTable->appendAcDbEntity(pLine); // Добавляем линию в блок-запись
		pLine->close(); // Закрываем объект линии после добавления
	}
}

void AddDetails(const TCHAR * s_DictName, const TCHAR * s_RecordName) {

	StarDict dict(s_DictName);
	AcDbDictionary* pStarlDict = dict.Get(AcDb::kForWrite);
	if (!pStarlDict) {
		return;
	}

	AcDbObjectId objId;
	if (pStarlDict->getAt(s_RecordName, objId) == Acad::eOk) {
		acutPrintf(L"\nEvent: Details already assign to that 'Star' object.");
		return;
	}

	std::unique_ptr<Star> upStarDet;

	upStarDet = std::make_unique<Star>(1, 5, L"Yellow");

	if (pStarlDict->setAt(s_RecordName, upStarDet.get(), objId) != Acad::eOk) {
		acutPrintf(L"\nError: Can't set record to Star dictionary");
		return;
	}

	acutPrintf(L"\nEvents: Create record to Star dictionary");
	upStarDet->close();
	upStarDet.release();
}

void RemoveDetails(const TCHAR* s_DictName, const TCHAR* s_RecordName) {

	AcDbObject* pObject;

	GetRefObject(pObject, AcDb::kForWrite);
	if (!pObject) {
		acutPrintf(L"\nError: Can't get reference object");
		return;
	}

	ExtensionDict extDict(pObject);
	AcDbDictionary* pExtDict = extDict.Get(AcDb::kForWrite);
	if (!pExtDict) {
		return;
	}

	StarDict dict(pExtDict, s_DictName);
	AcDbDictionary* pStarlDict = dict.Get(AcDb::kForWrite);
	if (!pStarlDict) {
		return;
	}

	AcDbObjectId objId;
	if (pStarlDict->getAt(s_RecordName, objId) == Acad::eKeyNotFound) {
		acutPrintf(L"\nWarning: No details assigned to that object");
		return;
	}

	AcDbObject* pObj;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite) != Acad::eOk) {
		acutPrintf(L"\nWarning: Can't to open the object detail");
		return;
	}

	pObj->erase();
	pObj->close();

	if (pStarlDict->numEntries() == 0) {
		pStarlDict->erase();
	}

	if (pExtDict->numEntries() == 0) {
		pExtDict->erase();
	}

	acutPrintf(L"\nEvent: Detail was removed");
}

void ListDetails(const TCHAR* s_DictName, const TCHAR* s_RecordName) {

	StarDict dict(s_DictName);
	AcDbDictionary* pStarlDict = dict.Get(AcDb::kForRead);
	if (!pStarlDict) {
		return;
	}

	AcDbObjectId objId;
	if (pStarlDict->getAt(s_RecordName, objId) == Acad::eKeyNotFound) {
		acutPrintf(L"\nWarning: No details assigned to that object");
		return;
	}

	AcDbObject* pObj;
	Acad::ErrorStatus es;
	if ((es = acdbOpenAcDbObject(pObj, objId, AcDb::kForRead)) != Acad::eOk) {
		acutPrintf(L"\nWarning: Can't to open the object detail");
		return;
	}

	Star* pStar = Star::cast(pObj);
	if (pStar) {
		Adesk::Int32 i;
		pStar->GetID(i);
		acutPrintf(_T("\nStar ID: %d"), i);
		pStar->GetNumberCorners(i);
		acutPrintf(_T("\nThe Number of corners of the Star: %d"), i);
		TCHAR* str = nullptr;
		pStar->GetColor(str);
		acutPrintf(_T("\nStar's color: %s"), str);
	}
	pStar->close();
}

Dictionary::~Dictionary()
{
	try {
		if (m_pDictionary) {
			m_pDictionary->close();
		}
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError code: %e\n", e);
	}
}

AcDbDictionary* Dictionary::Get(AcDb::OpenMode mode)
{
	if (!m_pDictionary) {
		return nullptr;
	}

	if (mode == AcDb::kForRead) {
		return m_pDictionary;
	}

	m_pDictionary->upgradeOpen();
	return m_pDictionary;
}

ExtensionDict::ExtensionDict()
{
	AcDbObjectId objectId;
	AcDbObject* pObject;

	GetRefObject(pObject, AcDb::kForWrite);
	if (!pObject) {
		acutPrintf(L"\nError: Can't get reference object");
		return;
	}

	// Create Extension Dictionary for referense object if it not already
	objectId = pObject->extensionDictionary();
	if (objectId == AcDbObjectId::kNull) {
		if (pObject->createExtensionDictionary() != Acad::eOk) {
			pObject->close();
			acutPrintf(L"\nError: Can't create extension dictionary");
			return;
		}
		objectId = pObject->extensionDictionary();
		acutPrintf(L"\nEvent: Create extension dictionary");
	}
	pObject->close();

	Acad::ErrorStatus es = acdbOpenAcDbObject((AcDbObject*&)m_pDictionary, objectId,
		AcDb::kForWrite, Adesk::kTrue);

	if (es != Acad::eOk) {
		throw std::runtime_error("Can't open extension dictionary");
	}

	if (m_pDictionary->isErased())
		m_pDictionary->erase(Adesk::kFalse);
}

ExtensionDict::ExtensionDict(AcDbObject*& pObject)
{
	AcDbObjectId objectId;

	// Create Extension Dictionary for referense object if it not already
	objectId = pObject->extensionDictionary();
	if (objectId == AcDbObjectId::kNull) {
		if (pObject->createExtensionDictionary() != Acad::eOk) {
			pObject->close();
			acutPrintf(L"\nError: Can't create extension dictionary");
			return;
		}
		objectId = pObject->extensionDictionary();
		acutPrintf(L"\nEvent: Create extension dictionary");
	}
	pObject->close();

	Acad::ErrorStatus es = acdbOpenAcDbObject((AcDbObject*&)m_pDictionary, objectId,
		AcDb::kForWrite, Adesk::kTrue);

	if (es != Acad::eOk) {
		throw std::runtime_error("Can't open extension dictionary");
	}

	if (m_pDictionary->isErased())
		m_pDictionary->erase(Adesk::kFalse);
}

StarDict::StarDict(const TCHAR* strDictName)
{
	ExtensionDict ed;
	AcDbDictionary* pExtDict;
	if (!(pExtDict = ed.Get(AcDb::kForWrite))) {
		return;
	}

	Acad::ErrorStatus error = pExtDict->getAt(strDictName, m_pDictionary);

	if (error == Acad::eKeyNotFound) {
		AcDbObjectId objId;
		std::unique_ptr<AcDbDictionary> upStarDict = std::make_unique<AcDbDictionary>();
		pExtDict->upgradeOpen();
		if (pExtDict->setAt(strDictName, upStarDict.get(), objId)
			!= Acad::eOk) {
			acutPrintf(_T("\nError: Can't create %s"), strDictName);
			return;
		}
		m_pDictionary = upStarDict.get();
		upStarDict.release();
		acutPrintf(_T("\nEvent: Created %s"), strDictName);
	}
}

StarDict::StarDict(AcDbDictionary* pExtDict, const TCHAR* strDictName)
{
	Acad::ErrorStatus error = pExtDict->getAt(strDictName, m_pDictionary);

	if (error == Acad::eKeyNotFound) {
		AcDbObjectId objId;
		std::unique_ptr<AcDbDictionary> upStarDict = std::make_unique<AcDbDictionary>();
		pExtDict->upgradeOpen();
		if (pExtDict->setAt(strDictName, upStarDict.get(), objId)
			!= Acad::eOk) {
			acutPrintf(_T("\nError: Can't create %s"), strDictName);
			return;
		}
		m_pDictionary = upStarDict.get();
		upStarDict.release();
		acutPrintf(_T("\nEvent: Created %s"), strDictName);
	}
}
