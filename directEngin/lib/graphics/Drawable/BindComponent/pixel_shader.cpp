#include "pixel_shader.h"
#include "cstdmf/string_converter.h"

PixelShader::PixelShader(Graphics& gfx,const std::string& path) : 
	_resourceID(path)
{
	INFOMAN(gfx);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	auto wPath = stringHelper::to_wstring(path);
	GFX_THROW_INFO(D3DReadFileToBlob(wPath.c_str(), &pBlob));
	GFX_THROW_INFO(gfx.getDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &p_pPixelShader));
}


void PixelShader::bind(Graphics& gfx) noexcept
{
	gfx.getContext()->PSSetShader(p_pPixelShader.Get(), nullptr, 0u);
}
