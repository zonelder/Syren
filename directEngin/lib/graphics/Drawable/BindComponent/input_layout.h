#pragma once
#include "IBindable.h"

struct ElementDesc
{
	std::string semanticName;
	UINT semanticIndex;
	DXGI_FORMAT format;
	UINT inputSlot;
	UINT alignedByteOffset;
	D3D11_INPUT_CLASSIFICATION inputSlotClass;
	UINT instanceDataStepRate;
};

class InputLayout : public IBindable
{
	using LayoutElements = std::vector<ElementDesc>;
public:
	InputLayout() = default;
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexBytecode);
	void bind(Graphics& gfx) noexcept override;

	const LayoutElements& getElements() const noexcept { return _layout; };

	size_t maxInputSlot() const noexcept { return _maxSlot; };
protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> p_pInputLayout = nullptr;
	LayoutElements _layout;
	size_t _maxSlot;
};

