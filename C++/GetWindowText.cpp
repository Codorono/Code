//###########################################################################
//
// Implementation of get window text functionality.
//
// Copyright (c) Paul MacDonald. All rights reserved.
//
//###########################################################################

#include "StdAfx.h"

#include <WinApi/Window.h>

//======================================================================================================================

std::wstring Pw::GetWindowTextW(HWND hWnd, size_t zBufferSize)
{
	std::wstring strResult;

	//get complete text

	if (zBufferSize == SIZE_MAX)
	{
		//allocate stack buffer

		std::array<wchar_t, _PMAC_STACK_ALLOC_WCHARS> arrStackBuffer;

		int nBufferSize{ static_cast<int>(arrStackBuffer.size()) };

		//get window text

		::SetLastError(ERROR_SUCCESS);

		int nResultSize{ ::GetWindowTextW(hWnd, arrStackBuffer.data(), nBufferSize) };

		//failure or empty window

		if (nResultSize == 0)
		{
			DWORD dwLastError{ ::GetLastError() };

			//failure

			if (dwLastError != ERROR_SUCCESS)
			{
				THROW_WIN32(dwLastError);
			}
		}

		//success

		else if ((nResultSize + 1) < nBufferSize)
		{
			strResult.assign(arrStackBuffer.data(), nResultSize);
		}

		//stack buffer too small

		else
		{
			//loop until it fits

			for (;;)
			{
				//get window text length

				::SetLastError(ERROR_SUCCESS);

				int nTextLength{ ::GetWindowTextLengthW(hWnd) };

				//failure or empty window

				if (nTextLength == 0)
				{
					DWORD dwLastError{ ::GetLastError() };

					//failure

					if (dwLastError != ERROR_SUCCESS)
					{
						THROW_WIN32(dwLastError);
					}

					//empty window

					else
					{
						break;
					}
				}

				//success

				else
				{
					//increase buffer size

					nBufferSize = nTextLength + 2;

					//allocate heap buffer

					std::unique_ptr<wchar_t[]> ptrHeapBuffer{ std::make_unique_for_overwrite<wchar_t[]>(nBufferSize) };

					//get window text

					::SetLastError(ERROR_SUCCESS);

					nResultSize = ::GetWindowTextW(hWnd, ptrHeapBuffer.get(), nBufferSize);

					//failure or empty window

					if (nResultSize == 0)
					{
						DWORD dwLastError{ ::GetLastError() };

						//failure

						if (dwLastError != ERROR_SUCCESS)
						{
							THROW_WIN32(dwLastError);
						}

						//empty window

						else
						{
							break;
						}
					}

					//success

					else if ((nResultSize + 1) < nBufferSize)
					{
						strResult.assign(ptrHeapBuffer.get(), nResultSize);

						break;
					}
				}
			}
		}
	}

	//get partial text

	else if (zBufferSize != 0)
	{
		//resize and overwrite

		strResult.resize_and_overwrite
		(
			zBufferSize - 1,
			[&](wchar_t* pchString, size_t zStringSize)
			{
				::SetLastError(ERROR_SUCCESS);

				int nResultSize{ ::GetWindowTextW(hWnd, pchString, static_cast<int>(zStringSize + 1)) };

				if (nResultSize == 0)
				{
					DWORD dwLastError{ ::GetLastError() };

					if (dwLastError != ERROR_SUCCESS)
					{
						THROW_WIN32(dwLastError);
					}
				}

				return nResultSize;
			}
		);
	}

	return strResult;
}

//======================================================================================================================
