#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <string>


namespace stringHelper
{

	std::wstring to_wstring(const char* str);
	std::wstring to_wstring(const std::string& str);


	std::string to_string(const wchar_t* str);
	std::string to_string(const std::wstring& str);
}
