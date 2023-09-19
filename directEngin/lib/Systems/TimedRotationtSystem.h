#pragma once
#include "../common/Scene/BaseSystem.h"

class TimedRotationSystem:public BaseSystem
{
public:
	void onUpdate(SceneManager& scene, float time) override;
};

