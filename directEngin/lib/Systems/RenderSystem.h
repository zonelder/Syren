#pragma once
#include "../component/Render.h"
#include "../component/Transform.h"
#include "../graphics/Graphics.h"

class RenderSystem
{
public:

	void OnFrame(Render& r, Graphics& gfx, Transform& transform);
};

