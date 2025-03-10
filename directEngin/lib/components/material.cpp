#include "Material.h"
#include "common/Scene/scene_manager.h"



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
		pTexture = SceneContext::pResources()->getDefaultTexture();
	}
	pTexture->bind(gfx);

}