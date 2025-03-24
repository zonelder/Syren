#pragma once
#include <string>
#include "serialization/base.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

SERIALIZE
struct Text
{
	SERIALIZE_FIELD std::wstring content{};
	HFONT font{ nullptr };
	SERIALIZE_FIELD float size = 40.0f;
	SERIALIZE_FIELD int color[4];
};