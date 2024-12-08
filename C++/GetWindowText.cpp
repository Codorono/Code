//###########################################################################
//
// Implementation of get window text functionality.
//
// Copyright (c) Paul MacDonald. All rights reserved.
//
//###########################################################################

#include "StdAfx.h"

#include <WinApi/Window.h>

#include <Text/Window.h>

//======================================================================================================================

std::wstring Ps::GetWindowTextW(HWND hWnd, size_t zMaxSize)
{
	std::wstring strResult;

	//get buffer size

	if (zMaxSize == SIZE_MAX)
	{
		zMaxSize = Pw::GetWindowTextLengthW(hWnd);
	}

	//ensure not empty

	if (zMaxSize != 0)
	{
		//get window text

		strResult.resize_and_overwrite
		(
			zMaxSize,
			[&](wchar_t* pchString, size_t zStringSize)
			{
				return Pw::GetWindowTextW(hWnd, pchString, zStringSize + 1);
			}
		);
	}

	return strResult;
}

//======================================================================================================================
