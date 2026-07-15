//###########################################################################
//
// Implementation of get current directory functionality.
//
// Copyright (c) Paul MacDonald. All rights reserved.
//
//###########################################################################

#include "StdAfx.h"

#include <WinApi/FilePath.h>

//======================================================================================================================

std::wstring Pw::GetCurrentDirectoryW()
{
	std::wstring strResult;

	//allocate stack buffer

	std::array<wchar_t, _PMAC_STACK_ALLOC_WCHARS> arrStackBuffer;

	DWORD dwBufferSize{ static_cast<DWORD>(arrStackBuffer.size()) };

	//get current directory

	DWORD dwResultSize{ ::GetCurrentDirectoryW(dwBufferSize, arrStackBuffer.data()) };

	//failure

	if (dwResultSize == 0)
	{
		THROW_LAST_ERROR();
	}

	//success

	else if (dwResultSize < dwBufferSize)
	{
		strResult.assign(arrStackBuffer.data(), dwResultSize);
	}

	//stack buffer too small

	else
	{
		//loop until it fits

		for (;;)
		{
			//increase buffer size

			dwBufferSize = dwResultSize;

			//allocate heap buffer

			std::unique_ptr<wchar_t[]> ptrHeapBuffer{ std::make_unique_for_overwrite<wchar_t[]>(dwBufferSize) };

			//get current directory

			dwResultSize = ::GetCurrentDirectoryW(dwBufferSize, ptrHeapBuffer.get());

			//failure

			if (dwResultSize == 0)
			{
				THROW_LAST_ERROR();
			}

			//success

			else if (dwResultSize < dwBufferSize)
			{
				strResult.assign(ptrHeapBuffer.get(), dwResultSize);

				break;
			}
		}
	}

	return strResult;
}

//======================================================================================================================
