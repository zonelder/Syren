#pragma once
#include <string>

struct Text
{
	std::wstring content{};
	HFONT font{ nullptr };
	float size = 40.0f;
};