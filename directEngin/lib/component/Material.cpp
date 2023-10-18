#include "Material.h"




void Material::init(Graphics& gfx)
{
	_pixelShader = PixelShader(gfx,pixelShader);
	_vertexShader = VertexShader(gfx,vertexShader);
	ID3DBlob* pBlob = _vertexShader.getBytecode();

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
	{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	_inputLayer = InputLayout(gfx, ied, pBlob);
}


void Material::bind(Graphics& gfx)
{
	_pixelShader.bind(gfx);
	_vertexShader.bind(gfx);
	_inputLayer.bind(gfx);
}