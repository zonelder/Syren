#pragma once
#include "../../Graphics.h"
#include "../../GraphicsThrowMacros.h"

class IBindable
{
public:
	virtual void bind(Graphics& gfx) noexcept = 0;
	virtual ~IBindable() = default;
};

