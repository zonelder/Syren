#pragma once
#include <string>
#include "serialization/component_serializer.h"

SERIALIZE
struct Text
{
	SERIALIZE_FIELD std::wstring content{};
	HFONT font{ nullptr };
	SERIALIZE_FIELD float size = 40.0f;
};