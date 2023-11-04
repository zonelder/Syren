#include "Material.h"




Material::Material(Graphics& gfx,const wchar_t* sVertexShader, const wchar_t* sPixelShader)
	:
	vertexShader(sVertexShader),
	pixelShader(sPixelShader),
	_texture(Texture(gfx, L"test_texture.dds"))
{
	_pixelShader = PixelShader(gfx,pixelShader);
	_vertexShader = VertexShader(gfx,vertexShader);
	//_texture = Texture(gfx, L"test_texture.png");
	ID3DBlob* pBlob = _vertexShader.getBytecode();

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
	{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},//DXGI_FORMAT_R32G32B32_FLOAT
	{ "TEXCORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	_inputLayer = InputLayout(gfx, ied, pBlob);


}


void Material::bind(Graphics& gfx)
{
	_inputLayer.bind(gfx);
	_vertexShader.bind(gfx);
	_pixelShader.bind(gfx);
	_texture.bind(gfx);
}