#pragma once
#include "../Graphics.h"

class IDrawable
{
public:

	IDrawable() = default;
	virtual void Draw(Graphics&) = 0;
};

