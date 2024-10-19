#pragma once
#include <string>
#include "serialization/base.h"

SERIALIZE
struct Text
{
	SERIALIZE_FIELD std::wstring content{};
	HFONT font{ nullptr };
	SERIALIZE_FIELD float size = 40.0f;
};