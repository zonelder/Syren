#pragma once
#include "../common/Scene/base_system.h"

using MaterialPtr = std::shared_ptr<Material>;

class CellGameSystem : public BaseSystem
{
public:

	void onInit(SceneManager& scene) override;
	void onUpdate(SceneManager& scene, float time) override;
private:

	static DirectX::XMFLOAT3 newBazis(const DirectX::XMFLOAT3&, const DirectX::XMMATRIX& mat);

	MaterialPtr p_selectMat;
	MaterialPtr p_deselectMat;
};