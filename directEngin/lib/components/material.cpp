#include "Material.h"




Material::Material(Graphics& gfx) :
	pTexture(nullptr)
{
	//_texture = Texture(gfx, L"test_texture.png");
}

void Material::bind(Graphics& gfx)
{

	if (pPixelShader)
		pPixelShader->bind(gfx);
	if (pVertexShader)
		pVertexShader->bind(gfx);

	if(pTexture)
		pTexture->bind(gfx);
	else
	{
		//TODO replace this
		static TexturePtr  pEmptyTexture = std::make_shared<Texture>(gfx, nullptr);
		pEmptyTexture->set(gfx, nullptr);
		pEmptyTexture->bind(gfx);
	}

}