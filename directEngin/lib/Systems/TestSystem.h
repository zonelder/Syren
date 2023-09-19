#pragma once
#include "../common/Scene/BaseSystem.h"

class TestSystem:public BaseSystem
{
public:
	void onUpdate(SceneManager& scene, float time) override;
};

