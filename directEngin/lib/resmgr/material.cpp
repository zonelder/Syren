#include "pch.h"
#include "material.h"
#include "scene/scene_manager.h"
#include "scene_context.h"

Material::Material(Graphics& gfx) :
	pTexture(nullptr)
{}

void Material::bind(Graphics& gfx)
{

	if (pPixelShader)
		pPixelShader->bind(gfx);
	if (pVertexShader)
		pVertexShader->bind(gfx);

	if (!pTexture)//in case some smart guy forget to set texture
	{
		pTexture = SceneContext::resources().getDefaultTexture();
	}
	pTexture->bind(gfx);

}