#include "input_layout.h"


InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexBytecode) :_maxSlot(0)
{

	INFOMAN(gfx);
	GFX_THROW_INFO(gfx.getDevice()->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexBytecode->GetBufferPointer(),
		pVertexBytecode->GetBufferSize(),
		&p_pInputLayout
	));

	
	for(const auto& e : layout)
	{
		_layout.push_back({ e.SemanticName, e.SemanticIndex, e.Format, e.InputSlot, e.AlignedByteOffset, e.InputSlotClass, e.InstanceDataStepRate });
		if(e.InputSlot > _maxSlot)
		{
			_maxSlot = e.InputSlot;
		}
	}
}

void InputLayout::bind(Graphics& gfx) noexcept
{
	gfx.getContext()->IASetInputLayout(p_pInputLayout.Get());

}
