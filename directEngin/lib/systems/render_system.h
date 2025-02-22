#pragma once
#include "../common/Scene/base_system.h"
#include "components/render.h"
#include "components/transform.h"
#include "../graphics/Graphics.h"
#include "components/parent.h"
#include "graphics/Drawable/BindComponent/dynamic_buffer.h"

struct BufferSlot 
{
	std::unique_ptr<DynamicBuffer> buffer;
	UINT elementSize;
	UINT slot;
};

enum class SemanticType {
	Position,
	UV,
	Normal,
	Color,
	Unknown,
	// ... другие типы
};

class RenderSystem:public BaseSystem
{
public:
	using WithComponents = filters::With<Parent, Render, Transform>;
	using RenderView = filters::ComponentView <WithComponents, filters::Without<>>;
	RenderSystem();
	void onFrame(SceneManager&) override;
	void onUpdate(SceneManager&, float dt) override;

	void bindMesh(MeshPtr pMesh,const InputLayout& layout);

	void registerBuffer(SemanticType semantic, const ElementDesc& desc);

private:
	void DeepRender(RenderView& view, const DirectX::XMMATRIX& viewProjection, EntityID id);
	void renderOne(Render& render, Transform& transform, const DirectX::XMMATRIX& viewProjection);
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_colorConstantBuffer;
	DirectX::XMMATRIX _wvp;
	VertexConstantBuffer<DirectX::XMMATRIX> _vertexConstantBuffer;

	std::unordered_map<SemanticType, BufferSlot> _buffers;
	IndexBuffer _indexBuffer;
};

