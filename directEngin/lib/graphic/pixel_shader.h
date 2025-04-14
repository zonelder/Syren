#pragma once
#include "IBindable.h"
#include <memory>

class PixelShader:public IBindable
{
public:
	PixelShader() = default;
	PixelShader(Graphics&,const std::string& path);
	void bind(Graphics&) noexcept override;

	const std::string& resourceID() const noexcept
	{
		return _resourceID;
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> p_pPixelShader = nullptr;
private:
	std::string _resourceID;

};


using PixelShaderPtr = std::shared_ptr<PixelShader>;