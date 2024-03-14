#pragma once

AcDbDatabase* GetActiveDB();

AcDbDictionaryPointer GetNamedDict(AcDbDatabase* pDatabase);

void AddDictToNamedDict(const TCHAR* sDictName, AcDbDictionaryPointer& pNamedDict, AcDbDictionaryPointer& pNewDict);

void RemoveDict(const TCHAR* sDictName, AcDbDictionaryPointer& pNamedDict);

void AddRecordToDict(const TCHAR* sDictName, const TCHAR* sRecord);

void RemoveRecordFromDict(const TCHAR* sDictName, const TCHAR* sRecord);

void GetAllRecordsFromDict(const TCHAR* sDictName);