#pragma once
#include "../common/Scene/base_system.h"


class FPSCounterSystem : public BaseSystem
{
public:
	void onUpdate(SceneManager& scene, float time) override;
	void onFrame(SceneManager& scene) override;
};