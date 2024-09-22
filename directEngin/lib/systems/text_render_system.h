#pragma once
#include "../common/Scene/base_system.h"
#include "FW1FontWrapper.h"


class TextRenderSystem:public BaseSystem
{
public:

	TextRenderSystem(Graphics& gfx);

	void onFrame(SceneManager& scene) override;

	~TextRenderSystem();
private:
	IFW1FontWrapper* p_fontWrapper;
};

