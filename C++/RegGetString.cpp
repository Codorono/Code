//###########################################################################
//
// Implementation of registry get string functionality.
//
// Copyright (c) Paul MacDonald. All rights reserved.
//
//###########################################################################

#include "StdAfx.h"

#include <WinApi/Registry.h>

//======================================================================================================================


bool RegGetString(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName,
	DWORD dwFlags, std::wstring& strValueData);

bool RegGetString(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName,
	DWORD dwFlags, DWORD& dwValueType, std::wstring& strValueData);

std::wstring RegGetStringDefault(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName,
	DWORD dwFlags, std::wstring_view stvValueDefault = L""sv);

std::wstring RegGetStringDefault(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName,
	DWORD dwFlags, DWORD& dwValueType, std::wstring_view stvValueDefault = L""sv);






//======================================================================================================================

bool Pw::RegGetString(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName, DWORD dwFlags,
	wchar_t* pszValueData, size_t& zValueSize)
{
	DWORD dwValueType;

	return Pw::RegGetString(hParentKey, pszSubkeyName, pszValueName, dwFlags, dwValueType, pszValueData, zValueSize);
}

//======================================================================================================================

bool Pw::RegGetString(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName, DWORD dwFlags,
	DWORD& dwValueType, wchar_t* pszValueData, size_t& zValueSize)
{
	bool fResult{ false };

	//get value size

	DWORD dwValueSize{ static_cast<DWORD>(zValueSize * sizeof(wchar_t)) };

	//get value data

	LSTATUS lStatus{ ::RegGetValueW(hParentKey, pszSubkeyName, pszValueName, dwFlags, &dwValueType, pszValueData,
		&dwValueSize) };

	if (lStatus == ERROR_SUCCESS)
	{
		zValueSize = dwValueSize / sizeof(wchar_t);

		fResult = true;
	}

	else if (lStatus != ERROR_FILE_NOT_FOUND)
	{
		THROW_WIN32(lStatus);
	}

	return fResult;
}

//======================================================================================================================

bool Pw::RegGetString(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName,
	DWORD dwFlags, std::wstring& strValueData)
{
	DWORD dwValueType;

	return Pw::RegGetString(hParentKey, pszSubkeyName, pszValueName, dwFlags, dwValueType, strValueData);
}

//======================================================================================================================

bool Pw::RegGetString(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName,
	DWORD dwFlags, DWORD& dwValueType, std::wstring& strValueData)
{
	hParentKey;
	pszSubkeyName;
	pszValueName;
	dwFlags;
	dwValueType;
	strValueData;

	return false;
}

//======================================================================================================================

std::wstring Pw::RegGetStringDefault(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName,
	DWORD dwFlags, std::wstring_view stvValueDefault)
{
	DWORD dwValueType;
	
	return Pw::RegGetStringDefault(hParentKey, pszSubkeyName, pszValueName, dwFlags, dwValueType,
		stvValueDefault);
}

//======================================================================================================================

std::wstring Pw::RegGetStringDefault(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName,
	DWORD dwFlags, DWORD& dwValueType, std::wstring_view stvValueDefault)
{					  
	std::wstring strValueData;

	if (!Pw::RegGetString(hParentKey, pszSubkeyName, pszValueName, dwFlags, dwValueType, strValueData))
	{
		strValueData = stvValueDefault;
	}	

	return strValueData;
}

//======================================================================================================================
