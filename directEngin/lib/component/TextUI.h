#pragma once
#include <string>
#include "directxmath.h"


/// @brief Component that represent data for rendering text line on UI Layer
struct TextUI
{

	/// @brief Text that neet to be rendered
	std::wstring content;

	/// @brief posotion of up-left corner of text
	DirectX::XMFLOAT2 position;

};