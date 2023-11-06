#pragma once
#include "../common/Scene/BaseSystem.h"

class ParentSystem: public BaseSystem
{
public:
	void onFrame(SceneManager&) override;
};

