#include "vertex_shader.h"
#include "cstdmf/string_converter.h"

VertexShader::VertexShader(Graphics& gfx, const std::string& path) : 
	_resourceID(path)
{
	INFOMAN(gfx);
	auto wPath = stringHelper::to_wstring(path);
	GFX_THROW_INFO(D3DReadFileToBlob(wPath.c_str(), &p_pBytecodeBlob));
	GFX_THROW_INFO(gfx.getDevice()->CreateVertexShader(p_pBytecodeBlob->GetBufferPointer(), p_pBytecodeBlob->GetBufferSize(), nullptr, &p_pVertexShader));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
	{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},//DXGI_FORMAT_R32G32B32_FLOAT
	{"TEXCORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,20,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	_inputLayer = InputLayout(gfx, ied, p_pBytecodeBlob.Get());
}


void VertexShader::bind(Graphics& gfx) noexcept
{
	gfx.getContext()->VSSetShader(p_pVertexShader.Get(), nullptr, 0u);
	_inputLayer.bind(gfx);
}