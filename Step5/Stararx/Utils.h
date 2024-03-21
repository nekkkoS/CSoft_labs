#pragma once
//TODO: переделать на ifdef
#pragma comment(lib, "../x64/Debug/Stardbx.lib")
//#pragma comment(lib, "../x64/Release/Stardbx.lib")


#include "StdAfx.h"
#include "../Stardbx/Star.h"

void GetRefObject(AcDbObject*& pObject, AcDb::OpenMode mode);

void AddStarBlock(AcDbBlockTableRecord* pBlockTable);

void AddDetails(const TCHAR * s_DictName, const TCHAR * s_RecordName);
void RemoveDetails(const TCHAR* s_DictName, const TCHAR* s_RecordName);
void ListDetails(const TCHAR* s_DictName, const TCHAR* s_RecordName);


class Dictionary {
protected:
	AcDbDictionary* m_pDictionary{};

public:
	~Dictionary();
	AcDbDictionary* Get(AcDb::OpenMode mode = AcDb::kForRead);
};

class ExtensionDict : public Dictionary {
public:
	ExtensionDict();
	ExtensionDict(AcDbObject*& pObject);
};

class StarDict : public Dictionary {
public:
	StarDict(const TCHAR* strDictName);
	StarDict(AcDbDictionary* pExtDict, const TCHAR* strDictName);
};