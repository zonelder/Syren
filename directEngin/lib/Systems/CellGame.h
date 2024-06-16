#pragma once
#include "../common/Scene/BaseSystem.h"

using MaterialPtr = std::shared_ptr<Material>;

class CellGameSystem : public BaseSystem
{
public:

	void onInit(SceneManager& scene) override;
	void onUpdate(SceneManager& scene, float time) override;
private:

	MaterialPtr p_selectMat;
	MaterialPtr p_deselectMat;
};