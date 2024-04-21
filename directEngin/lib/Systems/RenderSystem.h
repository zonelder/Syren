#pragma once
#include "../common/Scene/BaseSystem.h"
#include "../component/Render.h"
#include "../component/Transform.h"
#include "../graphics/Graphics.h"
#include "../component/Parent.h"
#include "../component/text.h"

class RenderSystem:public BaseSystem
{
public:
	RenderSystem(Graphics&);
	void onFrame(SceneManager&) override;
	void onUpdate(SceneManager&, float dt) override;

private:
	void DeepRender(Graphics& gfx, Transform& cam, ComponentPool<Render>& rendres, ComponentPool<Transform>& trs, ComponentPool<Parent>& parents, EntityID id);
	void renderOne(Render& render, Graphics& gfx, Transform& transform, const Transform& camTr);
	//void renderOne(Render& render, Graphics& gfx, Transform& transform,const Transform& camTr);
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_colorConstantBuffer;
};

