#include "StdAfx.h"
#include "EdStarReactor.h"
# define M_PI 3.14159265358979323846  /* pi */

//-----------------------------------------------------------------------------
EdStarReactor::EdStarReactor(const bool autoInitAndRelease) : AcEditorReactor(), mbAutoInitAndRelease(autoInitAndRelease) {
	if (autoInitAndRelease) {
		if (acedEditor)
			acedEditor->addReactor(this);
		else
			mbAutoInitAndRelease = false;
	}
}

//-----------------------------------------------------------------------------
EdStarReactor::~EdStarReactor() {
	Detach();
}

//-----------------------------------------------------------------------------
void EdStarReactor::Attach() {
	Detach();
	if (!mbAutoInitAndRelease) {
		if (acedEditor) {
			acedEditor->addReactor(this);
			mbAutoInitAndRelease = true;
		}
	}
}

void EdStarReactor::Detach() {
	if (mbAutoInitAndRelease) {
		if (acedEditor) {
			acedEditor->removeReactor(this);
			mbAutoInitAndRelease = false;
		}
	}
}

AcEditor* EdStarReactor::Subject() const {
	return (acedEditor);
}

bool EdStarReactor::IsAttached() const {
	return (mbAutoInitAndRelease);
}

void EdStarReactor::commandWillStart(const ACHAR* cmdStr) {
	// Создание карты команд и соответствующих сообщений
	std::map<std::wstring, std::wstring> mapwszCommandToMessage = {
		{L"MOVE", L"\nReceived MOVE command"},
		{L"ROTATE", L"\nReceived ROTATE command"},
		{L"SCALE", L"\nReceived SCALE command"},
	};

	acutPrintf(_T("\nCommand start"));

	auto it = mapwszCommandToMessage.find(std::wstring(cmdStr));
	if (it != mapwszCommandToMessage.end()) {
		acutPrintf(it->second.c_str());

		DocVars.docData().m_bEditCommand = true;
		DocVars.docData().m_bDoRepositioning = false;
		DocVars.docData().m_aIdChangedObjects.setLogicalLength(0);
		DocVars.docData().m_aPt3dStarPositions.setLogicalLength(0);
	}
}

void EdStarReactor::commandEnded(const ACHAR* cmdStr) {
	// Проверка активности отслеживаемой команды
	if (!DocVars.docData().m_bEditCommand)
		return;

	DocVars.docData().m_bEditCommand = false;
	// Отключение реактора объекта
	DocVars.docData().m_bDoRepositioning = true;

	// Смещение и поворот каждого измененного объекта
	for (int i = 0; i < DocVars.docData().m_aIdChangedObjects.length(); ++i) {
		AcDbObjectPointer<AcDbBlockReference> pInsert(DocVars.docData().m_aIdChangedObjects.at(i), AcDb::kForWrite);
		if (pInsert.openStatus() != Acad::eOk) {
			acutPrintf(_T("\nFailed to open object %d"), i + 1);
			return;
		}

		// Смещение вправо на 500 единиц
		AcGeVector3d vecDeltaRight(500.0, 0.0, 0.0);
		AcGePoint3d p3dNewPos = pInsert->position() + vecDeltaRight;
		pInsert->setPosition(p3dNewPos);

		// Получение текущего угла поворота и добавление 90 градусов (в радианах)
		double dCurrentRotation = pInsert->rotation();
		// Поворот на 90 градусов (pi/2 радиан)
		double dNewRotation = dCurrentRotation + (M_PI / 2);
		pInsert->setRotation(dNewRotation);

		acutPrintf(_T("\nBlock has been moved right by 500 units and rotated 90 degrees."));
	}
	acutPrintf(_T("\nCommand end"));
}