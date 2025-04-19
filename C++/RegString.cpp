//###########################################################################
//
// Implementation of registry string functionality.
//
// Copyright (c) Paul MacDonald. All rights reserved.
//
//###########################################################################

#include "StdAfx.h"

#include <WinApi/Registry.h>

#include <Registry/Registry.h>

//======================================================================================================================

bool Ps::RegGetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwFlags, wchar_t* pszValueData,
	size_t& zValueSize)
{
	DWORD dwValueType;

	return Ps::RegGetString(hKey, nullptr, pszValueName, dwFlags, dwValueType, pszValueData, zValueSize);
}

//======================================================================================================================

bool Ps::RegGetString(HKEY hParentKey, const wchar_t* pszSubkey, const wchar_t* pszValueName, DWORD dwFlags,
	wchar_t* pszValueData, size_t& zValueSize)
{
	DWORD dwValueType;

	return Ps::RegGetString(hParentKey, pszSubkey, pszValueName, dwFlags, dwValueType, pszValueData, zValueSize);
}

//======================================================================================================================

bool Ps::RegGetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwFlags, DWORD& dwValueType,
	wchar_t* pszValueData, size_t& zValueSize)
{
	return Ps::RegGetString(hKey, nullptr, pszValueName, dwFlags, dwValueType, pszValueData, zValueSize);
}

//======================================================================================================================

bool Ps::RegGetString(HKEY hParentKey, const wchar_t* pszSubkey, const wchar_t* pszValueName, DWORD dwFlags,
	DWORD& dwValueType, wchar_t* pszValueData, size_t& zValueSize)
{
	bool fResult{ false };

	if (dwFlags == 0)
	{
		dwFlags = RRF_RT_REG_SZ;
	}

	size_t zValueBytes{ zValueSize * sizeof(wchar_t) };

	size_t zResultBytes{ zValueBytes };

	if (Pw::RegGetValueW(hParentKey, pszSubkey, pszValueName, dwFlags, dwValueType, pszValueData, zResultBytes))
	{
		if (zResultBytes > zValueBytes)
		{
			THROW_WIN32(ERROR_MORE_DATA);
		}

		zValueSize = (zResultBytes / sizeof(wchar_t)) - 1;

		fResult = true;
	}

	return fResult;
}

//======================================================================================================================

bool Ps::RegGetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwFlags, std::wstring& strValueData)
{
	DWORD dwValueType;

	return Ps::RegGetString(hKey, nullptr, pszValueName, dwFlags, dwValueType, strValueData);
}

//======================================================================================================================

bool Ps::RegGetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwFlags, DWORD& dwValueType,
	std::wstring& strValueData)
{
	return Ps::RegGetString(hKey, nullptr, pszValueName, dwFlags, dwValueType, strValueData);
}

//======================================================================================================================

bool Ps::RegGetString(HKEY hParentKey, const wchar_t* pszSubkey, const wchar_t* pszValueName, DWORD dwFlags,
	std::wstring& strValueData)
{
	DWORD dwValueType;

	return Ps::RegGetString(hParentKey, pszSubkey, pszValueName, dwFlags, dwValueType, strValueData);
}

//======================================================================================================================

bool Ps::RegGetString(HKEY hParentKey, const wchar_t* pszSubkey, const wchar_t* pszValueName, DWORD dwFlags,
	DWORD& dwValueType, std::wstring& strValueData)
{
	bool fResult{ false };

	if (dwFlags == 0)
	{
		dwFlags = RRF_RT_REG_SZ;
	}

	//allocate stack buffer

	std::array<wchar_t, _PMAC_STACK_ALLOC_CHARS> arrStackBuffer;

	size_t zStackBytes{ arrStackBuffer.size() * sizeof(wchar_t) };

	size_t zResultBytes{ zStackBytes };

	//get registry value

	if (Pw::RegGetValueW(hParentKey, pszSubkey, pszValueName, dwFlags, dwValueType, arrStackBuffer.data(),
		zResultBytes))
	{
		//success

		if (zResultBytes <= zStackBytes)
		{
			//might have shrinkage

			strValueData.assign(arrStackBuffer.data(), std::wcslen(arrStackBuffer.data()));

			fResult = true;
		}

		//buffer too small

		else
		{
			//loop until it fits or not found

			for (;;)
			{
				//allocate heap buffer

				size_t zHeapBytes{ zResultBytes };

				std::unique_ptr<wchar_t[]> ptrHeapBuffer{ std::make_unique_for_overwrite<wchar_t[]>(zHeapBytes / sizeof(wchar_t)) };

				//get registry value

				if (!Pw::RegGetValueW(hParentKey, pszSubkey, pszValueName, dwFlags, dwValueType, ptrHeapBuffer.get(),
					zResultBytes))
				{
					//value not found

					break;
				}

				//success

				else if (zResultBytes <= zHeapBytes)
				{
					//might have shrinkage

					strValueData.assign(ptrHeapBuffer.get(), std::wcslen(ptrHeapBuffer.get()));

					fResult = true;

					break;
				}
			}
		}
	}

	return fResult;
}

//======================================================================================================================

void Ps::RegSetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwValueType, const wchar_t* pszValueData,
	size_t zValueSize)
{
	Ps::RegSetString(hKey, nullptr, pszValueName, dwValueType, pszValueData, zValueSize);
}

//======================================================================================================================

void Ps::RegSetString(HKEY hParentKey, const wchar_t* pszSubkey, const wchar_t* pszValueName, DWORD dwValueType,
	const wchar_t* pszValueData, size_t zValueSize)
{
	if (zValueSize == SIZE_MAX)
	{
		zValueSize = std::wcslen(pszValueData);
	}

	else if (*(pszValueData + zValueSize) != L'\0')
	{
		THROW_HR(E_INVALIDARG);
	}

	size_t zValueBytes{ (zValueSize + 1) * sizeof(wchar_t) };

	Pw::RegSetKeyValueW(hParentKey, pszSubkey, pszValueName, dwValueType, pszValueData, zValueBytes);
}

//======================================================================================================================

void Ps::RegSetString(HKEY hKey, const wchar_t* pszValueName, DWORD dwValueType, const std::wstring& strValueData)
{
	Ps::RegSetString(hKey, nullptr, pszValueName, dwValueType, strValueData);
}

//======================================================================================================================

void Ps::RegSetString(HKEY hParentKey, const wchar_t* pszSubkey, const wchar_t* pszValueName, DWORD dwValueType,
	const std::wstring& strValueData)
{
	if (dwValueType == 0)
	{
		dwValueType = REG_SZ;
	}

	Pw::RegSetKeyValueW(hParentKey, pszSubkey, pszValueName, dwValueType, strValueData.data(),
		(strValueData.size() + 1) * sizeof(wchar_t));
}

//======================================================================================================================
