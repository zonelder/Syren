#include "pixel_shader.h"


PixelShader::PixelShader(Graphics& gfx,const std::wstring& path)
{
	INFOMAN(gfx);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
	GFX_THROW_INFO(gfx.getDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &p_pPixelShader));
}


void PixelShader::bind(Graphics& gfx) noexcept
{
	gfx.getContext()->PSSetShader(p_pPixelShader.Get(), nullptr, 0u);
}
