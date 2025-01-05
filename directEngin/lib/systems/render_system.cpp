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
	
}

void RenderSystem::renderOne(Render& render,Transform& transform,const Transform& camTr)
{
	Graphics& gfx = *SceneContext::pGfx();
	auto* context = gfx.getContext();
	MeshInternal mesh(gfx,render.pMesh.get());//maybe we can create an instance once and just ypdate the buffers
	INFOMAN(gfx);

	///update transform buffer
	D3D11_MAPPED_SUBRESOURCE msr;
	auto pConstantBuffer = transform.vertexConstantBuffer.p_pConstantBuffer;
	auto FinalView = DirectX::XMMatrixTranspose(transform.orientationMatrix * camTr.orientationMatrix);
	GFX_THROW_INFO(context->Map(
		pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	));
	memcpy(msr.pData, &FinalView, sizeof(FinalView));
	context->Unmap(pConstantBuffer.Get(), 0u);

	// use binds
	transform.vertexConstantBuffer.bind(gfx);
	mesh.bind(gfx);

	// general material color
	context->UpdateSubresource(p_colorConstantBuffer.Get(), 0, nullptr, &(render.pMaterial->color), sizeof(DirectX::XMFLOAT4), 0);
	context->PSSetConstantBuffers(1u, 1u, p_colorConstantBuffer.GetAddressOf());

	render.pMaterial->bind(gfx);
	render.topology.bind(gfx);
	//draw

	gfx.DrawIndexed(render.pMesh->indices.size()/*TODO change to index count data in mesh*/, render.pMesh->startIndex);
}

void RenderSystem::DeepRender(RenderView& view,Transform& cam,EntityID id )
{

	auto& r = view.get<Render>(id);
	if (r.is_rendered)
		return;


	//before render object we should render its parent
	if (view.has<Parent>(id))
	{
		auto p_id = view.get<Parent>(id).parent;
		DeepRender(view, cam,p_id);
	}

	renderOne(r, view.get<Transform>(id), cam);

	
	r.is_rendered = true;
}

#pragma optimize("", off)
void RenderSystem::onFrame(SceneManager& scene)
{

	auto& camTr = scene.getCamera().transform;
	RenderView& view = scene.view<WithComponents>();
	for (auto [entt,p,r,tr] : view)
	{

		DeepRender(view, camTr,entt);
		
	}

	auto& commponView = scene.view<filters::With<Render, Transform>, filters::Without<Parent>>();
	for (auto [antt, r, tr] : commponView)
	{
		auto color = r.pMaterial->color;
		renderOne(r, tr, camTr);
	}

}

void RenderSystem::onUpdate(SceneManager& scene, float t)
{

	auto& rs = scene.getPool<Render>();
	for (auto& r : rs)
	{
		r.is_rendered = false;
	}

}
