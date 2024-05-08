#include "Texture.h"
#include "DDSTextureLoader.h"



Texture::Texture(Graphics& gfx)
{
	INFOMAN(gfx);

	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
	sampDesc.MipLODBias = 0;

	GFX_THROW_INFO(gfx.getDevice()->CreateSamplerState(&sampDesc, &_pSampleState));
}


Texture::Texture(Graphics& gfx, const wchar_t* path) : Texture(gfx)
{
	set(gfx, path);
}


void Texture::set(Graphics& gfx, const wchar_t* path)
{
	if (!path)
	{
		path = L"resource/empty.dds";
	};
	INFOMAN(gfx);
	reset();
	_pPath = path;
	//if it throw unknown exeption - then its bad texture 
	// ///@see https://github.com/Microsoft/DirectXTK/wiki/DDSTextureLoader
	GFX_THROW_INFO(DirectX::CreateDDSTextureFromFile(gfx.getDevice(), path, nullptr, p_pTextureRV.GetAddressOf()));
}

void Texture::reset()
{
	p_pTextureRV.Reset();
	_pPath.clear();
}

void Texture::bind(Graphics& gfx) noexcept
{
	//gfx.getContext()->OMSetDepthStencilState(;
	gfx.getContext()->PSSetSamplers(0, 1, _pSampleState.GetAddressOf());
	gfx.getContext()->PSSetShaderResources(0, 1, p_pTextureRV.GetAddressOf());

}

std::wstring Texture::getPath()
{
	return _pPath;
}