#pragma once
#include "../common/Scene/BaseSystem.h"

class CameraController:public BaseSystem
{
public:
	void onUpdate(SceneManager& scene,float time) override;
};

