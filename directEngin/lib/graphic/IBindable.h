#pragma once
#include "graphic/graphic.h"
#include "graphic/graphics_throw_macros.h"

class IBindable
{
public:
	virtual void bind(Graphics& gfx) noexcept = 0;
	virtual ~IBindable() = default;
};

