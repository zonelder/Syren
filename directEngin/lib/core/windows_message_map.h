#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <unordered_map>
#include <Windows.h>
#include <string>

class WindowsMessageMap
{
public:
	WindowsMessageMap() noexcept;
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wp) const noexcept;
private:
	std::unordered_map<DWORD, std::string> map;
};