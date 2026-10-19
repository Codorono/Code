//###########################################################################
//
// Implementation of registry string functionality.
//
// Copyright (c) Paul MacDonald. All rights reserved.
//
//###########################################################################

#include "StdAfx.h"

#include <WinApi/Registry.h>

//======================================================================================================================

bool Pw::RegGetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwFlags, wchar_t* pszValueData,
	size_t& zValueSize)
{
	DWORD dwValueType;

	return Pw::RegGetString(hKey, nullptr, pszValueName, dwFlags, dwValueType, pszValueData, zValueSize);
}

//======================================================================================================================

bool Pw::RegGetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwFlags, DWORD& dwValueType,
	wchar_t* pszValueData, size_t& zValueSize)
{
	return Pw::RegGetString(hKey, nullptr, pszValueName, dwFlags, dwValueType, pszValueData, zValueSize);
}

//======================================================================================================================

bool Pw::RegGetString(HKEY hParentKey, const wchar_t* pszSubKeyName, const wchar_t* pszValueName, DWORD dwFlags,
	wchar_t* pszValueData, size_t& zValueSize)
{
	DWORD dwValueType;

	return Pw::RegGetString(hParentKey, pszSubKeyName, pszValueName, dwFlags, dwValueType, pszValueData, zValueSize);
}

//======================================================================================================================

bool Pw::RegGetString(HKEY hParentKey, const wchar_t* pszSubKeyName, const wchar_t* pszValueName, DWORD dwFlags,
	DWORD& dwValueType, wchar_t* pszValueData, size_t& zValueSize)
{
	bool fResult{ false };

	if (dwFlags == 0)
	{
		dwFlags = RRF_RT_REG_SZ;
	}

	//get value bytes

	DWORD dwValueBytes{ static_cast<DWORD>(zValueSize * sizeof(wchar_t)) };

	//get value data

	LSTATUS lStatus{ ::RegGetValueW(hParentKey, pszSubKeyName, pszValueName, dwFlags, &dwValueType, pszValueData,
		&dwValueBytes) };

	if (lStatus == ERROR_SUCCESS)
	{
		zValueSize = dwValueBytes / sizeof(wchar_t);

		fResult = true;
	}

	else if (lStatus != ERROR_FILE_NOT_FOUND)
	{
		THROW_WIN32(lStatus);
	}

	return fResult;
}

//======================================================================================================================

bool Pw::RegGetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwFlags, std::wstring& strValueData)
{
	DWORD dwValueType;

	return Pw::RegGetString(hKey, nullptr, pszValueName, dwFlags, dwValueType, strValueData);
}

//======================================================================================================================

bool Pw::RegGetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwFlags, DWORD& dwValueType,
	std::wstring& strValueData)
{
	return Pw::RegGetString(hKey, nullptr, pszValueName, dwFlags, dwValueType, strValueData);
}

//======================================================================================================================

bool Pw::RegGetString(HKEY hParentKey, const wchar_t* pszSubKeyName, const wchar_t* pszValueName,
	DWORD dwFlags, std::wstring& strValueData)
{
	DWORD dwValueType;

	return Pw::RegGetString(hParentKey, pszSubKeyName, pszValueName, dwFlags, dwValueType, strValueData);
}

//======================================================================================================================

bool Pw::RegGetString(HKEY hParentKey, const wchar_t* pszSubKeyName, const wchar_t* pszValueName, DWORD dwFlags,
	DWORD& dwValueType, std::wstring& strValueData)
{
	bool fResult{ false };

	if (dwFlags == 0)
	{
		dwFlags = RRF_RT_REG_SZ;
	}

	//allocate stack buffer

	std::array<BYTE, _PMAC_STACK_ALLOC_BYTES> arrStackBuffer;

	DWORD dwBufferSize{ static_cast<DWORD>(arrStackBuffer.size()) };

	//get value

	LSTATUS lStatus{ ::RegGetValueW(hParentKey, pszSubKeyName, pszValueName, dwFlags, &dwValueType,
		arrStackBuffer.data(), &dwBufferSize) };

	//not not found

	if (lStatus != ERROR_FILE_NOT_FOUND)
	{
		//success

		if (lStatus == ERROR_SUCCESS)
		{
			const wchar_t* pszValueData{ reinterpret_cast<const wchar_t*>(arrStackBuffer.data()) };

			//possible shrinkage

			strValueData.assign(pszValueData, std::wcslen(pszValueData));

			fResult = true;
		}

		//failure

		else if (lStatus != ERROR_MORE_DATA)
		{
			THROW_WIN32(lStatus);
		}

		//stack buffer too small

		else
		{
			//loop until result fits in buffer

			for (;;)
			{
				//allocate heap buffer

				std::unique_ptr<BYTE[]> ptrHeapBuffer{ std::make_unique_for_overwrite<BYTE[]>(dwBufferSize) };

				//get value

				lStatus = ::RegGetValueW(hParentKey, pszSubKeyName, pszValueName, dwFlags, &dwValueType, ptrHeapBuffer.get(),
					&dwBufferSize);

				//success

				if (lStatus == ERROR_SUCCESS)
				{
					const wchar_t* pszValueData{ reinterpret_cast<const wchar_t*>(ptrHeapBuffer.get()) };

					//possible shrinkage

					strValueData.assign(pszValueData, std::wcslen(pszValueData));

					fResult = true;

					break;
				}

				//not found

				else if (lStatus == ERROR_FILE_NOT_FOUND)
				{
					break;
				}

				//unknown error

				else if (lStatus != ERROR_MORE_DATA)
				{
					THROW_WIN32(lStatus);
				}
			}
		}
	}

	return fResult;
}

//======================================================================================================================

std::wstring Pw::RegGetStringDefault(HKEY hParentKey, const wchar_t* pszSubKeyName, const wchar_t* pszValueName,
	DWORD dwFlags, std::wstring_view stvValueDefault)
{
	DWORD dwValueType;
	
	return Pw::RegGetStringDefault(hParentKey, pszSubKeyName, pszValueName, dwFlags, dwValueType,
		stvValueDefault);
}

//======================================================================================================================

std::wstring Pw::RegGetStringDefault(HKEY hParentKey, const wchar_t* pszSubKeyName, const wchar_t* pszValueName,
	DWORD dwFlags, DWORD& dwValueType, std::wstring_view stvValueDefault)
{					  
	std::wstring strValueData;

	if (!Pw::RegGetString(hParentKey, pszSubKeyName, pszValueName, dwFlags, dwValueType, strValueData))
	{
		strValueData = stvValueDefault;
	}	

	return strValueData;
}

//======================================================================================================================

void Pw::RegSetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwValueType, const wchar_t* pszValueData,
	size_t zValueSize)
{
	Pw::RegSetString(hKey, nullptr, pszValueName, dwValueType, pszValueData, zValueSize);
}

//======================================================================================================================

void Pw::RegSetString(HKEY hParentKey, const wchar_t* pszSubKeyName, const wchar_t* pszValueName, DWORD dwValueType,
	const wchar_t* pszValueData, size_t zValueSize)
{
	if (dwValueType == 0)
	{
		dwValueType = REG_SZ;
	}

	if (zValueSize == SIZE_MAX)
	{
		zValueSize = std::wcslen(pszValueData);
	}

	else if (*(pszValueData + zValueSize) != L'\0')
	{
		THROW_HR(E_INVALIDARG);
	}

	size_t zValueBytes{ (zValueSize + 1) * sizeof(wchar_t) };

	Pw::RegSetValueW(hParentKey, pszSubKeyName, pszValueName, dwValueType, pszValueData, zValueBytes);
}

//======================================================================================================================

void Pw::RegSetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwValueType, const std::wstring& strValueData)
{
	Pw::RegSetString(hKey, nullptr, pszValueName, dwValueType, strValueData);
}

//======================================================================================================================

void Pw::RegSetString(HKEY hParentKey, const wchar_t* pszSubKeyName, const wchar_t* pszValueName, DWORD dwValueType,
	const std::wstring& strValueData)
{
	if (dwValueType == 0)
	{
		dwValueType = REG_SZ;
	}

	Pw::RegSetValueW(hParentKey, pszSubKeyName, pszValueName, dwValueType, strValueData.c_str(),
		(strValueData.length() + 1) * sizeof(wchar_t));
}

//======================================================================================================================
