#pragma once
#include "../common/Scene/base_system.h"

class CameraController:public BaseSystem
{
public:
	void onUpdate(SceneManager& scene,float time) override;
};

