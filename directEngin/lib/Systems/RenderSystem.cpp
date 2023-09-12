#include "RenderSystem.h"


void RenderSystem::OnFrame(Render& render,Graphics& gfx,Transform& transform)
{
	INFOMAN(gfx);

	///update transform buffer
	D3D11_MAPPED_SUBRESOURCE msr;
	GFX_THROW_INFO(gfx.getContext()->Map(
		render.pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	));
	memcpy(msr.pData, &transform.orientationMatrix, sizeof(transform.orientationMatrix));
	gfx.getContext()->Unmap(render.pConstantBuffer.Get(), 0u);


	// use binds
	for (auto& bind : render.pBinds)
	{

		bind->bind(gfx);
	}

	//draw

	gfx.DrawIndexed(render.mesh.indices);
}
