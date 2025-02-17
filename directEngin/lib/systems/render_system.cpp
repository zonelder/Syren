#include "render_system.h"

#include <iostream>



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
	MeshInternal mesh(gfx,render.pMesh.get());//maybe we can create an instance once and just update the buffers
	INFOMAN(gfx);

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
	mesh.bind(gfx);

	// general material color
	context->UpdateSubresource(p_colorConstantBuffer.Get(), 0, nullptr, &(render.pMaterial->color), sizeof(DirectX::XMFLOAT4), 0);
	context->PSSetConstantBuffers(1u, 1u, p_colorConstantBuffer.GetAddressOf());

	render.pMaterial->bind(gfx);
	render.topology.bind(gfx);
	//draw

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
		DeepRender(view, viewProjection,p_id);
	}

	renderOne(r, view.get<Transform>(id), viewProjection);
	r.is_rendered = true;//TODO remove
}

void RenderSystem::onFrame(SceneManager& scene)
{

	auto& cam = scene.getCamera();
	RenderView& view = scene.view<WithComponents>();
	auto viewProjection = cam.view() * cam.projection();
	for (auto [entt,p,r,tr] : view)
	{

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
