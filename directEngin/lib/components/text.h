#pragma once
#include <string>
#include "serialization/component_serializer.h"

SERIALIZE
struct Text
{
	std::wstring content{};
	HFONT font{ nullptr };
	SERIALIZE_FIELD float size = 40.0f;
};