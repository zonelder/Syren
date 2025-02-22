#include "render_system.h"

#include <iostream>
#include <map>

namespace
{
	class SemanticParser {
	public:
		static SemanticType parse(const char* semanticName, UINT index = 0) {
			if (!semanticName) return SemanticType::Unknown;

			// Преобразуем в нижний регистр для унификации
			const std::string semantic = ToLower(semanticName);

			// Удаляем завершающие цифры (например, "TEXCOORD0" -> "texcoord")
			const std::string base = semantic.substr(0, semantic.find_first_of("0123456789"));

			static const std::unordered_map<std::string, SemanticType> mapping = {
				{"position", SemanticType::Position},
				{"uv",		 SemanticType::UV},
				{"texcord",  SemanticType::UV},
				{"normal",   SemanticType::Normal},
				{"color",    SemanticType::Color}
			};

			auto it = mapping.find(base);
			return (it != mapping.end()) ? it->second : SemanticType::Unknown;
		}

	private:
		static std::string ToLower(const char* str) {
			std::string result(str);
			std::transform(result.begin(), result.end(), result.begin(),
				[](unsigned char c) { return std::tolower(c); });
			return result;
		}
	};

	static SemanticType parseSemantic(const std::string& semantic) {
		if (semantic == "POSITION") return SemanticType::Position;
		if (semantic == "TEXCOORD") return SemanticType::UV;
		if (semantic == "NORMAL") return SemanticType::Normal;
		if (semantic == "COLOR") return SemanticType::Color;
		throw std::runtime_error("Unknown semantic: " + semantic);
	}

	UINT getElementSize(SemanticType semantic)
	{
		switch (semantic) {
		case SemanticType::Position: return sizeof(Mesh::Position_t);
		case SemanticType::UV:		 return sizeof(Mesh::UV_t);
		case SemanticType::Normal:   return sizeof(Mesh::Normal_t);
		case SemanticType::Color:    return sizeof(Mesh::Color_t);
		default: throw std::runtime_error("Unknown semantic");
		}
	}

	const void* getMeshData(const Mesh& mesh, SemanticType semantic)
	{
		switch (semantic) {
		case SemanticType::Position: return mesh.vertexes.data();
		case SemanticType::UV:		 return mesh.uvs.data();
		case SemanticType::Normal:   return mesh.normals.data();
		case SemanticType::Color:    return mesh.colors.data();
		default: return nullptr;
		}
	}

	UINT getElementCount(const Mesh& mesh, SemanticType semantic)
	{
		switch (semantic) {
		case SemanticType::Position: return mesh.vertexes.size();
		case SemanticType::UV:		 return mesh.uvs.size();
		case SemanticType::Normal:   return mesh.normals.size();
		case SemanticType::Color:    return mesh.colors.size();
		default: return 0;
		}
	}

}


RenderSystem::RenderSystem()
{
	auto gfx = SceneContext::pGfx();
	INFOMAN((*gfx));
	D3D11_BUFFER_DESC constantBufferDesc = {};

	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	GFX_THROW_INFO(gfx->getDevice()->CreateBuffer(&constantBufferDesc, nullptr, &p_colorConstantBuffer));
	_vertexConstantBuffer =	 VertexConstantBuffer<DirectX::XMMATRIX>(*gfx,_wvp);
	
}

void RenderSystem::renderOne(Render& render,Transform& transform, const DirectX::XMMATRIX& viewProjection)
{
	Graphics& gfx = *SceneContext::pGfx();
	auto* context = gfx.getContext();
	INFOMAN(gfx);

	assert(render.pMesh);

	bool hasIndicies = !render.pMesh->indices.empty();
	_wvp = transform.orientationMatrix * viewProjection;
	_wvp = DirectX::XMMatrixTranspose(_wvp);
	///update transform buffer
	D3D11_MAPPED_SUBRESOURCE msr;
	auto pConstantBuffer = _vertexConstantBuffer.p_pConstantBuffer;
	GFX_THROW_INFO(context->Map(
		pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	));
	memcpy(msr.pData, &_wvp, sizeof(_wvp));
	context->Unmap(pConstantBuffer.Get(), 0u);

	// use binds
	_vertexConstantBuffer.bind(gfx);
	const auto& layout = render.pMaterial->pVertexShader->inputLayer();

	bindMesh(render.pMesh, layout);

	// general material color
	context->UpdateSubresource(p_colorConstantBuffer.Get(), 0, nullptr, &(render.pMaterial->color), sizeof(DirectX::XMFLOAT4), 0);
	context->PSSetConstantBuffers(1u, 1u, p_colorConstantBuffer.GetAddressOf());

	render.pMaterial->bind(gfx);
	render.topology.bind(gfx);
	//draw
	if (!hasIndicies)
	{
		context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		gfx.Draw(render.pMesh->vertexes.size(), 0);
		return;
	}
	_indexBuffer = IndexBuffer(gfx, render.pMesh->indices);
	_indexBuffer.bind(gfx);
	gfx.DrawIndexed(render.pMesh->indices.size()/*TODO change to index count data in mesh*/, render.pMesh->startIndex);
}

void RenderSystem::DeepRender(RenderView& view, const DirectX::XMMATRIX& viewProjection,EntityID id )
{

	auto& r = view.get<Render>(id);
	if (r.is_rendered)
		return;

	//before render object we should render its parent
	if (view.has<Parent>(id))
	{
		auto p_id = view.get<Parent>(id).parent;
		assert(p_id != id);
		DeepRender(view, viewProjection,p_id);
	}

	renderOne(r, view.get<Transform>(id), viewProjection);
	r.is_rendered = true;//TODO remove
}

void RenderSystem::onFrame(SceneManager& scene)
{

	static float time = 0.0f;
	time += 0.01f;
	float offset = sinf(time)*1.0f;
	auto& cam = scene.getCamera();
	RenderView& view = scene.view<WithComponents>();
	auto viewProjection = cam.view() * cam.projection();
	for (auto [entt,p,r,tr] : view)
	{
		view.get<Transform>(p.parent).position.x += offset;
		DeepRender(view, viewProjection, entt);
	}
	auto& commponView = scene.view<filters::With<Render, Transform>, filters::Without<Parent>>();
	for (auto [antt, r, tr] : commponView)
	{
		auto color = r.pMaterial->color;
		renderOne(r, tr, viewProjection);
	}

}

void RenderSystem::onUpdate(SceneManager& scene, float t)
{

	auto& rs = scene.getPool<Render>();
	for (auto& r : rs)
	{
		r.is_rendered = false;//TODO remove
	}

}

void RenderSystem::bindMesh(MeshPtr pMesh, const InputLayout& layout)
{
	auto pGfx = SceneContext::pGfx();
	// Собираем слоты и соответствующие буферы
	std::unordered_map<UINT, ID3D11Buffer*> slotToBuffer;
	std::unordered_map<UINT, UINT> slotToStride;

	for (const auto& elem : layout.getElements()) {
		SemanticType semantic = SemanticParser::parse(elem.semanticName.c_str());
		assert(semantic != SemanticType::Unknown);

		if (!_buffers.count(semantic)) 
		{
			registerBuffer(semantic, elem);
		}

		auto& bufferSlot = _buffers[semantic];
		const void* data = getMeshData(*pMesh, semantic);
		UINT count = getElementCount(*pMesh, semantic);

		bufferSlot.buffer->update(
			data,
			count,
			bufferSlot.elementSize
		);
		slotToBuffer[elem.inputSlot] = bufferSlot.buffer->get();
		slotToStride[elem.inputSlot] = bufferSlot.elementSize;

	}

	for (const auto& [slot, buffer] : slotToBuffer) {
		const UINT stride = slotToStride[slot];
		const UINT offset = 0;
		pGfx->getContext()->IASetVertexBuffers(
			slot,      // Точный слот из Input Layout
			1,         // Количество буферов (1)
			&buffer,   // Указатель на буфер
			&stride,
			&offset
		);
	}

}

void RenderSystem::registerBuffer(SemanticType semantic, const ElementDesc& desc) {
	UINT elementSize = getElementSize(semantic);
	_buffers[semantic] = { std::make_unique<DynamicBuffer>(*SceneContext::pGfx(), elementSize),elementSize,desc.inputSlot};
}
