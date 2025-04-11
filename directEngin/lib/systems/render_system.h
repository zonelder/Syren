#pragma once
#include "../common/Scene/base_system.h"
#include "components/render.h"
#include "components/transform.h"
#include "../graphics/Graphics.h"
#include "components/parent.h"
#include "graphics/Drawable/BindComponent/dynamic_buffer.h"
#include "graphics/geometric_buffer.h"

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

struct FinalPassCBData
{
	int selectedOutput; // 0: Альбедо, 1: Нормали, 2: Глубина
	// Выравнивание до 16 байт, если требуется.
	int pad[3];
};

class RenderSystem:public BaseSystem
{
public:
	using WithComponents = filters::With<Render, Transform>;
	using RenderView = filters::ComponentView <WithComponents, filters::Without<>>;
	RenderSystem();
	void onFrame(SceneManager&) override;
	void onUpdate(SceneManager&, float dt) override;

	void bindMesh(MeshPtr pMesh,const InputLayout& layout);

	void registerBuffer(SemanticType semantic, const ElementDesc& desc);

	void drawFinalPass();

private:
	void renderOne(Render& render, Transform& transform, const Matrix4x4& viewProjection);
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_colorConstantBuffer;
	Matrix4x4 _wvp;
	VertexConstantBuffer<DirectX::XMMATRIX> _vertexConstantBuffer;

	std::unordered_map<SemanticType, BufferSlot> _buffers;
	IndexBuffer _indexBuffer;

	VertexShaderPtr _pFinalVertexShader;
	PixelShaderPtr _pFinalPixelShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState > _pFinalSampler;
	GBuffer gBuffer_;
	FinalPassCBData _finalPassData;

};

