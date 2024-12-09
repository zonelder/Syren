#include "Material.h"




Material::Material(Graphics& gfx) :
	texture(Texture(gfx/*, L"test_texture.dds"*/))
{
	//_texture = Texture(gfx, L"test_texture.png");
}

void Material::bind(Graphics& gfx)
{

	if (pPixelShader)
		pPixelShader->bind(gfx);
	if (pVertexShader)
		pVertexShader->bind(gfx);
	texture.bind(gfx);
}