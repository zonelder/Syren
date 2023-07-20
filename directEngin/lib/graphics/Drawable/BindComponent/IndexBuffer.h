#pragma once
#include "IBindable.h"

class IndexBuffer:public IBindable
{

public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	void bind(Graphics&) noexcept override;
	UINT getCount() const noexcept;
protected:
	UINT p_count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_pIndexBuffer;
};

