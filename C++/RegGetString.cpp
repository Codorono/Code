//###########################################################################
//
// Implementation of registry get string functionality.
//
// Copyright (c) Paul MacDonald. All rights reserved.
//
//###########################################################################

#include "StdAfx.h"

#include <WinApi/Registry.h>

#include <Registry/Registry.h>

//======================================================================================================================

std::optional<std::wstring> Ps::RegGetString(HKEY hParentKey, const wchar_t* pszSubkeyName,
	const wchar_t* pszValueName, DWORD dwFlags)
{
	DWORD dwValueType;

	return Ps::RegGetString(hParentKey, pszSubkeyName, pszValueName, dwFlags, dwValueType);
}

//======================================================================================================================

std::optional<std::wstring> Ps::RegGetString(HKEY hParentKey, const wchar_t* pszSubkeyName,
	const wchar_t* pszValueName, DWORD dwFlags, DWORD& dwValueType)
{
	std::optional<std::wstring> optResult;

	dwFlags |= RRF_RT_REG_SZ;

	//allocate stack buffer

	std::array<wchar_t, 1024> cStackBuffer;

	size_t zStackBytes{ cStackBuffer.size() * sizeof(wchar_t) };

	size_t zResultBytes{ zStackBytes };

	//get registry value

	if (Pw::RegGetValueW(hParentKey, pszSubkeyName, pszValueName, dwFlags, dwValueType, cStackBuffer.data(),
		zResultBytes))
	{
		//success

		if (zResultBytes <= zStackBytes)
		{
			//calculate string length in case of shrinkage

			optResult = std::wstring{ cStackBuffer.data(), std::wcslen(cStackBuffer.data()) };
		}

		//buffer too small

		else
		{
			//loop until it fits

			size_t zHeapBytes{ zResultBytes };

			for (;;)
			{
				//allocate heap buffer

				std::unique_ptr<wchar_t[]> cHeapBuffer{ std::make_unique_for_overwrite<wchar_t[]>(zHeapBytes / sizeof(wchar_t)) };

				zResultBytes = zHeapBytes;

				//get registry value

				if (!Pw::RegGetValueW(hParentKey, pszSubkeyName, pszValueName, dwFlags, dwValueType,
					cHeapBuffer.get(), zResultBytes))
				{
					//value not found

					break;
				}

				//success

				else if (zResultBytes <= zHeapBytes)
				{
					//calculate string length in case of shrinkage

					optResult = std::wstring{ cHeapBuffer.get(), std::wcslen(cHeapBuffer.get()) };

					break;
				}

				//enlarge heap buffer

				zHeapBytes = zResultBytes;
			}
		}
	}

	return optResult;
}

//======================================================================================================================

std::wstring Ps::RegGetStringDefault(HKEY hParentKey, const wchar_t* pszSubkeyName, const wchar_t* pszValueName,
	DWORD dwFlags, const wchar_t* pszValueDefault)
{
	std::wstring strResult;

	std::optional<std::wstring> optResult{ Ps::RegGetString(hParentKey, pszSubkeyName, pszValueName, dwFlags) };

	if (optResult)
	{
		strResult = *optResult;
	}

	else
	{
		strResult = pszValueDefault;
	}

	return strResult;
}

//======================================================================================================================
