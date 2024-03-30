#include "StdAfx.h"
#include "EdStarReactor.h"

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

void EdStarReactor::commandEnded(const ACHAR* cmdStr)
{
	// Was one of our monitored commands active
	if (DocVars.docData().m_bEditCommand == false)
		return;

	DocVars.docData().m_bEditCommand = false;

	// disable the object reactor
	// (so the object reactor knows that we are changing the object)
	DocVars.docData().m_bDoRepositioning = true;

	// Reset to previous position
	for (int i = 0; i < DocVars.docData().m_aIdChangedObjects.length(); ++i) {
		AcDbObjectPointer<AcDbBlockReference> pInsert(DocVars.docData().m_aIdChangedObjects.at(i), AcDb::kForWrite);
		if (pInsert.openStatus() != Acad::eOk) {
			acutPrintf(_T("\nFailed to open object %d"), i + 1);
			return;
		}

		AcGePoint3d p3dNewPos = pInsert->position();
		AcGePoint3d p3d_OldPos = DocVars.docData().m_aPt3dStarPositions.at(i);
		// Resetting the position to the original one
		if (p3dNewPos != p3d_OldPos) {
			pInsert->setPosition(p3d_OldPos);
			acutPrintf(_T("\nBlock has been reset to its original location."));
		}
	}
	acutPrintf(_T("\nCommand end"));
}
