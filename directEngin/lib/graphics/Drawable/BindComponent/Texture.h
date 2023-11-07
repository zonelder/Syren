#pragma once
#include "IBindable.h"
#include <iostream>

class Texture:public IBindable
{
public:

	Texture() = delete;
	Texture(Graphics& gfx);
	Texture(Graphics& gfx,const wchar_t* path);
	void bind(Graphics& gfx) noexcept override;
	void set(Graphics& gfx,const wchar_t* path);
	void reset();
	std::wstring getPath();


private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_pTextureRV;
	Microsoft::WRL::ComPtr<ID3D11Resource> p_pResorceTexture;
	Microsoft::WRL::ComPtr<ID3D11SamplerState > _pSampleState;
	std::wstring _pPath;
};