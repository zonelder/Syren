#pragma once
#include "../common/Scene/BaseSystem.h"

class RenderSystem:public BaseSystem
{
public:

	void onFrame(SceneManager&) override;
	void onUpdate(SceneManager&, float dt) override;
private:
	void renderOne(Render& render, Graphics& gfx, Transform& transform,const Transform& camTr);
};

