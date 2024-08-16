#pragma once
#include "components/transform.h"
#include "../common/Scene/BaseSystem.h"

class OrientationSystem:public BaseSystem
{

public:

	void onFrame(SceneManager& scene) override;
};

