#pragma once
#include "../common/Scene/base_system.h"
#include "math/vector3.h"

using MaterialPtr = std::shared_ptr<Material>;

class CellGameSystem : public BaseSystem
{
public:

	void onInit(SceneManager& scene) override;
	void onUpdate(SceneManager& scene, float time) override;
private:

	static Vector3 newBazis(const Vector3&, const DirectX::XMMATRIX& mat);

	MaterialPtr p_selectMat;
	MaterialPtr p_deselectMat;
};