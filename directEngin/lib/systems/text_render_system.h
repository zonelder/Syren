#pragma once
#include "scene/base_system.h"
#include "FW1FontWrapper.h"
#include "resmgr/scene_context.h"

class TextRenderSystem:public BaseSystem
{
public:

	TextRenderSystem(Graphics& gfx);

	void onFrame(SceneManager& scene) override;

	~TextRenderSystem();
private:
	IFW1FontWrapper* p_fontWrapper;
};

