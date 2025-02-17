#pragma once
#include "../common/Scene/base_system.h"
#include "components/render.h"
#include "components/transform.h"
#include "../graphics/Graphics.h"
#include "components/parent.h"

class RenderSystem:public BaseSystem
{
public:
	using WithComponents = filters::With<Parent, Render, Transform>;
	using RenderView = filters::ComponentView <WithComponents, filters::Without<>>;
	RenderSystem();
	void onFrame(SceneManager&) override;
	void onUpdate(SceneManager&, float dt) override;

private:
	void DeepRender(RenderView& view, const DirectX::XMMATRIX& viewProjection, EntityID id);
	void renderOne(Render& render, Transform& transform, const DirectX::XMMATRIX& viewProjection);
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_colorConstantBuffer;
	DirectX::XMMATRIX _wvp;
	VertexConstantBuffer<DirectX::XMMATRIX> _vertexConstantBuffer;
};

