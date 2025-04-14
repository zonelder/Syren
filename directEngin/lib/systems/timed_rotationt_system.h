#pragma once
#include "scene/base_system.h"

class TimedRotationSystem:public BaseSystem
{
public:
	void onUpdate(SceneManager& scene, float time) override;
};

