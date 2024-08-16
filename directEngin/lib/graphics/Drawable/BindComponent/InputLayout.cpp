#include "input_layout.h"


InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexBytecode)
{
	INFOMAN(gfx);
	GFX_THROW_INFO(gfx.getDevice()->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexBytecode->GetBufferPointer(),
		pVertexBytecode->GetBufferSize(),
		&p_pInputLayout
	));
}

void InputLayout::bind(Graphics& gfx) noexcept
{
	gfx.getContext()->IASetInputLayout(p_pInputLayout.Get());

}
