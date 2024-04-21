#include "RenderSystem.h"

#include <iostream>



RenderSystem::RenderSystem(Graphics& gfx)
{
	INFOMAN(gfx);
	D3D11_BUFFER_DESC constantBufferDesc = {};

	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	GFX_THROW_INFO(gfx.getDevice()->CreateBuffer(&constantBufferDesc, nullptr, &p_colorConstantBuffer));
	
}

void RenderSystem::renderOne(Render& render,Graphics& gfx,Transform& transform,const Transform& camTr)
{
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
	render.p_mesh->bind(gfx);

	// general material color
	gfx.getContext()->UpdateSubresource(p_colorConstantBuffer.Get(), 0, nullptr, &(render.p_material->color), sizeof(DirectX::XMFLOAT4), 0);
	gfx.getContext()->PSSetConstantBuffers(1u, 1u, p_colorConstantBuffer.GetAddressOf());

	render.p_material->bind(gfx);
	render.topology.bind(gfx);
	//draw

	gfx.DrawIndexed(render.p_mesh->indices);//74


	
}

void RenderSystem::DeepRender(Graphics& gfx,Transform& cam,ComponentPool<Render>& rendres, ComponentPool<Transform>& trs, ComponentPool<Parent>& parents,EntityID id )
{

	auto& r = rendres.getComponent(id);
	if (!trs.hasComponent(id) || r.is_rendered)
		return;
	//before render object we should render its parent

	if (parents.hasComponent(id))
	{
		auto p_id = parents.getComponent(id).parent;
		DeepRender(gfx, cam, rendres, trs, parents, p_id);
	}

	renderOne(r, gfx, trs.getComponent(id), cam);

	
	r.is_rendered = true;
}


void RenderSystem::onFrame(SceneManager& scene)
{
	Graphics& gfx = scene.getGraphic();
	auto& _transforms = scene.getPool<Transform>();
	auto& _renders = scene.getPool<Render>();
	auto& _parents = scene.getPool<Parent>();
	auto& camTr = scene.getCamera().transform;
	auto& _texts = scene.getPool<Text>();

	for (auto& entt :scene.getEntitiesWith<Transform,Render>())
	{

		DeepRender(gfx, camTr, _renders, _transforms, _parents, entt.getID());
		
	}

}

void RenderSystem::onUpdate(SceneManager& scene, float t)
{

	auto& rs = scene.getPool<Render>();
	for (auto& [entID, r] : rs)
	{

		r.is_rendered = false;
	}

}
