#include "Texture.h"
#include "DDSTextureLoader.h"

Texture::Texture(Graphics& gfx, const wchar_t* path)
{
	
	INFOMAN(gfx);

	//if it throw unknown exeption - then its bad texture 
	// ///@see https://github.com/Microsoft/DirectXTK/wiki/DDSTextureLoader
	GFX_THROW_INFO(DirectX::CreateDDSTextureFromFile(gfx.getDevice(),path,nullptr, p_pTextureRV.GetAddressOf()));

	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	GFX_THROW_INFO(gfx.getDevice()->CreateSamplerState(&sampDesc, &_pSampleState));
}


void Texture::bind(Graphics& gfx) noexcept
{
	gfx.getContext()->PSSetShaderResources(0, 1, p_pTextureRV.GetAddressOf());
	gfx.getContext()->PSSetSamplers(0, 1, _pSampleState.GetAddressOf());

}