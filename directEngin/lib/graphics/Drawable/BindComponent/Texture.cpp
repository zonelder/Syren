#include "Texture.h"
#include "graphics/graphics_throw_macros.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "resmngr/file_system.h"
#include "cstdmf/string_converter.h"


Texture::Texture(Graphics& gfx, const std::string& path) 
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
	_pPath = stringHelper::to_wstring(path);
	auto ext = fileSystem::getExtension(_pPath);
	if (ext == L"dds")
	{
		//if it throw unknown exeption - then its bad texture 
		// ///@see https://github.com/Microsoft/DirectXTK/wiki/DDSTextureLoader
		GFX_THROW_INFO(DirectX::CreateDDSTextureFromFile(gfx.getDevice(), _pPath.c_str(), nullptr, p_pTextureRV.GetAddressOf()));
	}
	else
	{
		GFX_THROW_INFO(DirectX::CreateWICTextureFromFile(gfx.getDevice(), gfx.getContext(), _pPath.c_str(), nullptr, p_pTextureRV.GetAddressOf()));
	}
}

void Texture::bind(Graphics& gfx) const
{
	gfx.getContext()->PSSetSamplers(0, 1, _pSampleState.GetAddressOf());
	gfx.getContext()->PSSetShaderResources(0, 1, p_pTextureRV.GetAddressOf());

}

std::wstring Texture::getPath()
{
	return _pPath;
}