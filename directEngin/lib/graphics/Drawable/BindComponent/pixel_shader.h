#pragma once
#include "IBindable.h"

class PixelShader:public IBindable
{
public:
	PixelShader() = default;
	PixelShader(Graphics&,const std::wstring& path);
	void bind(Graphics&) noexcept override;

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> p_pPixelShader = nullptr;

};

