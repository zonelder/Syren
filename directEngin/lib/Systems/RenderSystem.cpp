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

void RenderSystem::renderOne(Render& render,Graphics& gfx,Transform& transform,const Transform& camTr, MeshIternal* mesh)
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
	mesh->bind(gfx);

	// general material color
	gfx.getContext()->UpdateSubresource(p_colorConstantBuffer.Get(), 0, nullptr, &(render.p_material->color), sizeof(DirectX::XMFLOAT4), 0);
	gfx.getContext()->PSSetConstantBuffers(1u, 1u, p_colorConstantBuffer.GetAddressOf());

	render.p_material->bind(gfx);
	render.topology.bind(gfx);
	//draw

	gfx.DrawIndexed(render.p_mesh->IndexCount,render.p_mesh->startIndex);
}

void RenderSystem::DeepRender(SceneManager& scene,Transform& cam,Entity id )
{

	auto& r = scene.getComponent<Render>(id);
	if (r.is_rendered)
		return;


	//before render object we should render its parent
	if (scene.hasComponent<Parent>(id))
	{
		auto p_id = scene.getComponent<Parent>(id).parent;
		DeepRender(scene, cam,scene.getEntity(p_id));
	}

	renderOne(r, scene.getGraphic(), scene.getComponent<Transform>(id), cam, scene.getMeshData(r.p_mesh));

	
	r.is_rendered = true;
}


void RenderSystem::onFrame(SceneManager& scene)
{
	Graphics& gfx = scene.getGraphic();

	auto& camTr = scene.getCamera().transform;

	for (auto& entt :scene.getEntitiesWith<Transform,Render>())
	{

		DeepRender(scene, camTr,entt);
		
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
