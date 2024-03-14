#include "stdafx.h"
#include "utils.h"
#include <tchar.h>

#include <iostream>
#include <stdexcept>

AcDbDatabase* GetActiveDB() {
	AcDbDatabase* pDatabase = nullptr;
	try {
		pDatabase = acdbHostApplicationServices()->workingDatabase();
		if (!pDatabase) {
			acutPrintf(_T("\nNo active database."));
		}
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError accessing active database: %e\n", e);
	}

	return pDatabase;
}

AcDbDictionaryPointer GetNamedDict(AcDbDatabase* pDatabase) {
	AcDbDictionaryPointer pNamedDict = nullptr;
	try {
		pNamedDict.open(pDatabase->namedObjectsDictionaryId(), AcDb::kForWrite);
		if (pNamedDict.openStatus() != Acad::eOk) {
			acutPrintf(_T("\nError accessing named objects dictionary."));
		}
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError accessing active database: %e\n", e);
	}

	return pNamedDict;
}

void AddDictToNamedDict(const TCHAR* sDictName, AcDbDictionaryPointer& pNamedDict, AcDbDictionaryPointer& pNewDict) {
	try {
		Acad::ErrorStatus es = Acad::eOk;
		es = pNamedDict->setAt(sDictName, pNewDict.object(), pNewDict->id());
		if (es != Acad::eOk) {
			acutPrintf(_T("\nError creating dictionary."));
		}
		else {
			acutPrintf(_T("\nDictionary '%s' created successfully."), sDictName);
		}
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError code: %e\n", e);
	}
}

void RemoveDict(const TCHAR* sDictName, AcDbDictionaryPointer& pNamedDict) {
	try {
		AcDbObjectId dictId = AcDbObjectId::kNull;
		if (pNamedDict->getAt(sDictName, dictId) != Acad::eOk) {
			acutPrintf(_T("\nDictionary with name '%s' not found."), sDictName);
			return;
		}

		AcDbDictionaryPointer pDictToRemove(dictId, AcDb::kForWrite);
		if (pDictToRemove.openStatus() != Acad::eOk) {
			acutPrintf(_T("\nError accessing dictionary '%s'."), sDictName);
			return;
		}

		Acad::ErrorStatus es = pNamedDict->remove(sDictName);
		if (es != Acad::eOk) {
			acutPrintf(_T("\nError removing dictionary '%s'."), sDictName);
		}
		else {
			acutPrintf(_T("\nDictionary '%s' removed successfully."), sDictName);
		}
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError code: %e\n", e);
	}
}

void AddRecordToDict(const TCHAR* sDictName, const TCHAR* sRecord) {
	try {
		AcDbDatabase* pDatabase = GetActiveDB();
		AcDbDictionaryPointer pNamedDict = GetNamedDict(pDatabase);

		AcDbObjectId objId = AcDbObjectId::kNull;
		Acad::ErrorStatus es = pNamedDict->getAt(sDictName, objId);
		if (es != Acad::eOk) {
			if (es == Acad::eKeyNotFound) {
				acutPrintf(_T("\nDictionary with name '%s' not found."), sDictName);
			}
			else {
				acutPrintf(_T("\nError occurred while accessing the dictionary."));
			}
			return;
		}

		AcDbObjectPointer<AcDbObject> pObject(objId, AcDb::kForWrite);
		if (pObject.openStatus() != Acad::eOk) {
			acutPrintf(_T("\nCannot open the object for write"));
			return;
		}

		// Как скастить pObject в AcDbDictionaryPointer?
		AcDbDictionary* pTargetDict = AcDbDictionary::cast(pObject);

		if (pTargetDict == NULL) {
			acutPrintf(_T("\nEntry found in the NOD, but it is not a dictionary"));
			return;
		}

		if (pTargetDict->getAt(sRecord, objId) == Acad::eOk) {
			acutPrintf(_T("\nRecord '%s' already exists"), sRecord);
			return;
		}

		// Добавление записи в словарь
		AcDbObjectPointer<AcDbXrecord> pRecord;
		pRecord.create();
		if (pTargetDict->setAt(sRecord, pRecord, objId) != Acad::eOk) {
			acutPrintf(_T("\nFailed to add new record in the dictionary"));
			return;
		}

		acutPrintf(_T("\nRecord '%s' added to dictionary successfully"), sRecord);
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError code: %e\n", e);
	}
}

void RemoveRecordFromDict(const TCHAR* sDictName, const TCHAR* sRecord) {
	try {
		AcDbDatabase* pDatabase = GetActiveDB();
		AcDbDictionaryPointer pNamedDict = GetNamedDict(pDatabase);

		AcDbObjectId objId = AcDbObjectId::kNull;
		Acad::ErrorStatus es = pNamedDict->getAt(sDictName, objId);
		if (es != Acad::eOk) {
			if (es == Acad::eKeyNotFound) {
				acutPrintf(_T("\nKey '%s' not found in the NamedDict."), sDictName);
			}
			else {
				acutPrintf(_T("\nError occurred while accessing the dictionary."));
			}
			return;
		}

		AcDbObjectPointer<AcDbObject> pObject(objId, AcDb::kForWrite);
		if (pObject.openStatus() != Acad::eOk) {
			acutPrintf(_T("\nCannot open the object for write"));
			return;
		}

		// Как скастить pObject в AcDbDictionaryPointer?
		AcDbDictionary* pTargetDict = AcDbDictionary::cast(pObject);

		if (pTargetDict == NULL) {
			acutPrintf(_T("\nEntry found in the NOD, but it is not a dictionary"));
			return;
		}

		if (pTargetDict->getAt(sRecord, objId) != Acad::eOk) {
			acutPrintf(_T("\nRecord '%s' not found in the NamedDict"), sRecord);
			return;
		}

		// Удаление записи из словаря
		if (pTargetDict->remove(sRecord) != Acad::eOk) {
			acutPrintf(_T("\nFailed to remove record '%s' from the dictionary"), sRecord);
			return;
		}

		acutPrintf(_T("\nRecord '%s' removed from dictionary successfully"), sRecord);
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError code: %e\n", e);
	}
}

void GetAllRecordsFromDict(const TCHAR* sDictName) {
	try {
		AcDbDatabase* pDatabase = GetActiveDB();
		AcDbDictionaryPointer pNamedDict = GetNamedDict(pDatabase);

		AcDbObjectId objId = AcDbObjectId::kNull;
		if (pNamedDict.openStatus() == Acad::eOk && pNamedDict->getAt(sDictName, objId) == Acad::eOk) {
			AcDbObjectPointer<AcDbObject> pObject(objId, AcDb::kForRead);
			if (pObject.openStatus() == Acad::eOk) {
				// Как скастить pObject в AcDbDictionaryPointer?
				AcDbDictionary* pDict = AcDbDictionary::cast(pObject);
				if (pDict != nullptr) {
					std::unique_ptr<AcDbDictionaryIterator> pIter(pDict->newIterator());
					if (pIter != nullptr) {
						for (; !pIter->done(); pIter->next()) {
							acutPrintf(_T("*Record: %s\n"), pIter->name());
						}
					}
				}
			}
		}
		else {
			acutPrintf(_T("\nNo dictionary with name '%s'."), sDictName);
		}
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError code: %e\n", e);
	}
}
