//###########################################################################
//
// Implementation of get current directory functionality.
//
// Copyright (c) Paul MacDonald. All rights reserved.
//
//###########################################################################

#include "StdAfx.h"

#include <WinApi/FilePath.h>

#include <FileSys/FilePath.h>

//======================================================================================================================

std::wstring Ps::GetCurrentDirectoryW()
{
	std::wstring strResult;

	//allocate stack buffer

	std::array<wchar_t, _PMAC_STACK_ALLOC_CHARS> arrStackBuffer;

	//get current directory

	size_t zResultSize{ Pw::GetCurrentDirectoryW(arrStackBuffer.size(), arrStackBuffer.data()) };

	//success

	if (zResultSize < arrStackBuffer.size())
	{
		strResult.assign(arrStackBuffer.data(), zResultSize);
	}

	//buffer too small

	else
	{
		//loop until it fits

		for (;;)
		{
			//allocate heap buffer

			size_t zBufferSize{ zResultSize };

			std::unique_ptr<wchar_t[]> ptrHeapBuffer{ std::make_unique_for_overwrite<wchar_t[]>(zBufferSize) };

			//get current directory

			zResultSize = Pw::GetCurrentDirectoryW(zBufferSize, ptrHeapBuffer.get());

			//success

			if (zResultSize < zBufferSize)
			{
				strResult.assign(ptrHeapBuffer.get(), zResultSize);

				break;
			}
		}
	}

	return strResult;
}

//======================================================================================================================
