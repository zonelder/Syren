#pragma once
#include "../common/Scene/BaseSystem.h"
#include "../component/Render.h"
#include "../component/Transform.h"
#include "../graphics/Graphics.h"
#include "../component/Parent.h"

class RenderSystem:public BaseSystem
{
public:
	RenderSystem(Graphics&);
	void onFrame(SceneManager&) override;
	void onUpdate(SceneManager&, float dt) override;

private:
	void DeepRender(SceneManager& scene, Transform& cam, Entity id);
	void renderOne(Render& render, Graphics& gfx, Transform& transform, const Transform& camTr,MeshIternal* mesh);
	//void renderOne(Render& render, Graphics& gfx, Transform& transform,const Transform& camTr);
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_colorConstantBuffer;
};

