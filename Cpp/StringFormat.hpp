//###########################################################################
//
// Implementation of string format function.
//
//###########################################################################

#pragma once

#include <locale.h>
#include <cstdarg>
#include <string>

//==================================================================================================

namespace Px
{
	//==============================================================================================

	template <typename TChar = char>
	struct FormatTraits
	{
		static int GetSize(const char* pszFormat, std::va_list pArgList) noexcept
		{
			return ::_vscprintf(pszFormat, pArgList);
		}

		static int Format(char* pszBuffer, size_t uCapacity,
			const char* pszFormat, std::va_list pArgList) noexcept
		{
			return ::vsprintf_s(pszBuffer, uCapacity, pszFormat, pArgList);
		}
	};

	//==============================================================================================

	template <>
	struct FormatTraits<wchar_t>
	{
		static int GetSize(const wchar_t* pszFormat, std::va_list pArgList) noexcept
		{
			return ::_vscwprintf(pszFormat, pArgList);
		}

		static int Format(wchar_t* pszBuffer, size_t uCapacity,
			const wchar_t* pszFormat, std::va_list pArgList) noexcept
		{
			return ::vswprintf_s(pszBuffer, uCapacity, pszFormat, pArgList);
		}
	};

	//==============================================================================================

	template <>
	struct FormatTraits<char8_t>
	{
		static int GetSize(const char8_t* pszFormat, std::va_list pArgList) noexcept
		{
			_locale_t sUtf8Locale{ _wcreate_locale(LC_CTYPE, L".UTF-8") };

			return ::_vscprintf_l(reinterpret_cast<const char*>(pszFormat), sUtf8Locale, pArgList);
		}

		static int Format(char8_t* pszBuffer, size_t uCapacity,
			const char8_t* pszFormat, std::va_list pArgList) noexcept
		{
			_locale_t sUtf8Locale{ _wcreate_locale(LC_CTYPE, L".UTF-8") };

			return ::_vsprintf_s_l(reinterpret_cast<char*>(pszBuffer), uCapacity,
				reinterpret_cast<const char*>(pszFormat), sUtf8Locale, pArgList);
		}
	};

	//==============================================================================================

	template <typename TChar>
	inline auto FormatV(const TChar* pszFormat,
		std::va_list pArgList) noexcept -> std::basic_string<TChar>
	{
		using FormatTraits = Px::FormatTraits<TChar>;

		std::basic_string<TChar> strResult;

		//get format size

		int nFormatSize{ FormatTraits::GetSize(pszFormat, pArgList) };

		if (nFormatSize >= 0)
		{
			//format text

			strResult.resize(nFormatSize);

			nFormatSize = FormatTraits::Format(strResult.data(),
				strResult.size() + 1, pszFormat, pArgList);
		}

		//set result size

		if (nFormatSize < 0)
			nFormatSize = 0;

		strResult.resize(nFormatSize);

		return strResult;
	}

	//==============================================================================================

	template <typename TChar>
	inline auto Format(const TChar* pszFormat, ...) noexcept -> std::basic_string<TChar>
	{
		//get arg list

		std::va_list pArgList;
		va_start(pArgList, pszFormat);

		//format text

		std::basic_string<TChar> strResult{ Px::FormatV(pszFormat, pArgList) };

		//reset arg list

		va_end(pArgList);

		return strResult;
	}

	//==============================================================================================

	template <typename TChar>
	inline auto AppendFormat(std::basic_string<TChar>& strResult,
		const TChar* pszFormat, ...) noexcept -> std::basic_string<TChar>&
	{
		//get arg list

		std::va_list pArgList;
		va_start(pArgList, pszFormat);

		//format append string

		std::basic_string<TChar> strAppend{ Px::FormatV(pszFormat, pArgList) };

		//reset arg list

		va_end(pArgList);

		//append result

		return strResult.append(strAppend);
	}

	//==============================================================================================

} //namespace Px

//==================================================================================================
