#include "string_converter.h"

namespace stringHelper
{
	std::wstring to_wstring(const char* str)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
		std::wstring wstr(size, L'\0'); // Создаем строку с нужным размером

		MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr.data(), size);
		return wstr;
	}

	std::wstring to_wstring(const std::string& str)
	{
		return to_wstring(str.c_str());
	}

	std::string to_string(const wchar_t* str)
	{
		int size = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
		std::string retStr(size, '\0');
		WideCharToMultiByte(CP_UTF8, 0, str, -1, retStr.data(), size, nullptr, nullptr);
		return retStr;
	}

	std::string to_string(const std::wstring& str)
	{
		return to_string(str.c_str());
	}

}
