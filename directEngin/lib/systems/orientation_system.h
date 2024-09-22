#pragma once
#include "components/transform.h"
#include "../common/Scene/base_system.h"

class OrientationSystem:public BaseSystem
{

public:

	void onFrame(SceneManager& scene) override;
};

