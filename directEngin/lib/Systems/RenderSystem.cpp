#include "RenderSystem.h"

#include <iostream>



RenderSystem::RenderSystem(SceneManager& scene): _scene(scene)
{
	auto& gfx = scene.getGraphic();
	INFOMAN(gfx);
	D3D11_BUFFER_DESC constantBufferDesc = {};

	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	GFX_THROW_INFO(gfx.getDevice()->CreateBuffer(&constantBufferDesc, nullptr, &p_colorConstantBuffer));
	
}

void RenderSystem::renderOne(Render& render,Transform& transform,const Transform& camTr)
{
	Graphics& gfx = _scene.getGraphic();
	MeshIternal* mesh = _scene.getMeshData(render.p_mesh);
	INFOMAN(gfx);

	///update transform buffer
	D3D11_MAPPED_SUBRESOURCE msr;
	auto pConstantBuffer = transform.vertexConstantBuffer.p_pConstantBuffer;
	auto FinalView = DirectX::XMMatrixTranspose(transform.orientationMatrix * camTr.orientationMatrix);
	GFX_THROW_INFO(gfx.getContext()->Map(
		pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	));
	memcpy(msr.pData, &FinalView, sizeof(FinalView));
	gfx.getContext()->Unmap(pConstantBuffer.Get(), 0u);

	// use binds
	transform.vertexConstantBuffer.bind(gfx);
	mesh->bind(gfx);

	// general material color
	gfx.getContext()->UpdateSubresource(p_colorConstantBuffer.Get(), 0, nullptr, &(render.p_material->color), sizeof(DirectX::XMFLOAT4), 0);
	gfx.getContext()->PSSetConstantBuffers(1u, 1u, p_colorConstantBuffer.GetAddressOf());

	render.p_material->bind(gfx);
	render.topology.bind(gfx);
	//draw

	gfx.DrawIndexed(render.p_mesh->IndexCount,render.p_mesh->startIndex);
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


void RenderSystem::onFrame(SceneManager& scene)
{
	Graphics& gfx = scene.getGraphic();

	auto& camTr = scene.getCamera().transform;
	RenderView& view = scene.view<WithComponents>();
	for (auto [entt,p,r,tr] : view)
	{

		DeepRender(view, camTr,entt);
		
	}

	auto& commponView = scene.view<With<Render, Transform>, Without<Parent>>();
	for (auto [antt, r, tr] : commponView)
	{
		renderOne(r, tr, camTr);
	}

}

void RenderSystem::onUpdate(SceneManager& scene, float t)
{

	auto& rs = scene.getPool<Render>();
	//auto& view = scene.view<Render>();
	for (auto& r : rs)
	{

		r.is_rendered = false;
	}

}
