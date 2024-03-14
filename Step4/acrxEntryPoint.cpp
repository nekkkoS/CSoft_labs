#include "StdAfx.h"
#include "resource.h"
#include "utils.h"
#include <tchar.h>
#include <iostream>
#include <exception>

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep4App : public AcRxArxApp {

public:
	CStep4App() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	static void Step4_test() {

	}

	static void Step4_createDict() {
		AcDbDatabase* pDatabase = GetActiveDB();
		AcDbDictionaryPointer pNamedDict = GetNamedDict(pDatabase);


		TCHAR sDictName[MAX_PATH] = { 0 };
		acedGetString(0, _T("\nEnter name for the new dictionary: "), sDictName);

		if (pNamedDict->has(sDictName)) {
			acutPrintf(_T("\nDictionary with name '%s' already exists."), sDictName);
			return;
		}

		AcDbDictionaryPointer pNewDict;
		pNewDict.create();

		AddDictToNamedDict(sDictName, pNamedDict, pNewDict);
	}

	static void Step4_removeDict() {
		try {
			AcDbDatabase* pDatabase = GetActiveDB();

			AcDbDictionaryPointer pNamedDict = GetNamedDict(pDatabase);

			TCHAR sDictName[MAX_PATH] = { 0 };
			acedGetString(0, _T("\nEnter name of the dictionary to delete: "), sDictName);

			RemoveDict(sDictName, pNamedDict);

		}
		catch (const Acad::ErrorStatus& e) {
			acutPrintf(L"\nError code: %e\n", e);
		}
	}

	static void Step4_addRecordToDict() {
		TCHAR sDictName[MAX_PATH] = { 0 };
		acedGetString(0, _T("\nEnter name of dictionary: "), sDictName);

		TCHAR sRecord[MAX_PATH] = { 0 };
		acedGetString(0, _T("\nEnter record name to add: "), sRecord);

		AddRecordToDict(sDictName, sRecord);
	}

	static void Step4_removeRecordFromDict() {
		TCHAR sDictName[MAX_PATH] = { 0 };
		acedGetString(0, _T("\nEnter name of dictionary: "), sDictName);

		TCHAR sRecord[MAX_PATH] = { 0 };
		acedGetString(0, _T("\nEnter record name to remove: "), sRecord);

		RemoveRecordFromDict(sDictName, sRecord);
	}

	static void Step4_getAllRecordsFromDict() {
		TCHAR sDictName[MAX_PATH] = { 0 };
		acedGetString(0, _T("\nEnter name of dictionary: "), sDictName);

		GetAllRecordsFromDict(sDictName);
	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep4App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep4App, Step4, _test, TEST, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep4App, Step4, _createDict, create_Dict, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep4App, Step4, _removeDict, remove_Dict, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep4App, Step4, _addRecordToDict, add_Record_To_Dict, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep4App, Step4, _removeRecordFromDict, remove_Record_From_Dict, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep4App, Step4, _getAllRecordsFromDict, get_All_Records_From_Dict, ACRX_CMD_MODAL, NULL)