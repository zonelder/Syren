#include "RenderSystem.h"
#include "../component/Render.h"
#include "../component/Transform.h"
#include "../graphics/Graphics.h"
#include "../component/Parent.h"
#include <iostream>

void renderOne(Render& render,Graphics& gfx,Transform& transform,const Transform& camTr)
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
	render.p_material->bind(gfx);
	render.topology.bind(gfx);
	//draw

	gfx.DrawIndexed(render.p_mesh->indices);//74
}

void DeepRender(Graphics& gfx,Transform& cam,ComponentPool<Render>& rendres, ComponentPool<Transform>& trs, ComponentPool<Parent>& parents,EntityID id )
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
	std::cout << id;


	r.is_rendered = true;
}


void RenderSystem::onFrame(SceneManager& scene)
{
	Graphics& gfx = scene.getGraphic();
	std::cout << "----------\n";
	auto& _transforms = scene.getPool<Transform>();
	auto& _renders = scene.getPool<Render>();
	auto& _parents = scene.getPool<Parent>();
	auto& camTr = scene.getCamera().transform;


	for (auto& [entID, r] : _renders)
	{

		DeepRender(gfx, camTr, _renders, _transforms, _parents, entID);
	}


	std::cout << "----------\n";
}

void RenderSystem::onUpdate(SceneManager& scene, float t)
{

	auto& rs = scene.getPool<Render>();
	for (auto& [entID, r] : rs)
	{

		r.is_rendered = false;
	}


}