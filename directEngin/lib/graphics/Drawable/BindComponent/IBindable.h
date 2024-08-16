#pragma once
#include "../../graphics.h"
#include "../../graphics_throw_macros.h"

class IBindable
{
public:
	virtual void bind(Graphics& gfx) noexcept = 0;
	virtual ~IBindable() = default;
};

