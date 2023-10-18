#include "RenderSystem.h"
#include "../component/Render.h"
#include "../component/Transform.h"
#include "../graphics/Graphics.h"


void RenderSystem::renderOne(Render& render,Graphics& gfx,Transform& transform)
{
	INFOMAN(gfx);

	///update transform buffer
	D3D11_MAPPED_SUBRESOURCE msr;
	auto pConstantBuffer = transform.vertexConstantBuffer.p_pConstantBuffer;
	GFX_THROW_INFO(gfx.getContext()->Map(
		pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	));
	memcpy(msr.pData, &transform.orientationMatrix, sizeof(transform.orientationMatrix));
	gfx.getContext()->Unmap(pConstantBuffer.Get(), 0u);


	// use binds
	transform.vertexConstantBuffer.bind(gfx);
	render.p_mesh->bind(gfx);
	render.p_material->bind(gfx);
	render.topology.bind(gfx);
	//draw

	gfx.DrawIndexed(render.p_mesh->indices);//74
}


void RenderSystem::onFrame(SceneManager& scene)
{
	Graphics& gfx = scene.getGraphic();

	auto& _transforms = scene.getPool<Transform>();
	auto& _renders = scene.getPool<Render>();

	for (auto& [entID, r] : _renders)
	{
		if (!_transforms.hasComponent(entID))
			continue;
		renderOne(r, gfx, _transforms.getComponent(entID));
	}
}
