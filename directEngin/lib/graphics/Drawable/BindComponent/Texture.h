#pragma once
#include "IBindable.h"
#include <iostream>
#include <memory>

class Texture:public IBindable
{
public:

	Texture() = delete;
	Texture(Graphics& gfx);
	Texture(Graphics& gfx,const wchar_t* path);
	void bind(Graphics& gfx) noexcept override;
	void set(Graphics& gfx,const wchar_t* path);

	std::wstring getPath();
private:
	void reset();

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_pTextureRV;
	Microsoft::WRL::ComPtr<ID3D11Resource> p_pResorceTexture;
	Microsoft::WRL::ComPtr<ID3D11SamplerState > _pSampleState;
	std::wstring _pPath;
};


using TexturePtr = std::shared_ptr<Texture>;