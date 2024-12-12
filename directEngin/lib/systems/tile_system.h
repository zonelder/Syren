#pragma once
#include "common/Scene/base_system.h"
#include "components/material.h"


class TileSystem : public BaseSystem
{
public:
	void onInit(SceneManager& scene) override;
	void onUpdate(SceneManager& scene,float time) override;

private:

	MaterialPtr p_selectMat;
	MaterialPtr p_deselectMat;
};