#pragma once
#include "IBindable.h"

class InputLayout : public IBindable
{
public:
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexBytecode);
	void bind(Graphics& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> p_pInputLayout;
};

