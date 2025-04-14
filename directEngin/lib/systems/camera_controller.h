#pragma once
#include "scene/base_system.h"

class CameraController:public BaseSystem
{
public:
	void onUpdate(SceneManager& scene,float time) override;
};

