#pragma once
#include "common/Scene/base_system.h"



class TileSystem : public BaseSystem
{
public:
	void onUpdate(SceneManager& scene,float time) override;
};