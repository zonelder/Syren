#pragma once
#include "IBindable.h"

class VertexBuffer:public IBindable
{
public:
	template<class T>
	VertexBuffer(Graphics& gfx, const std::vector<T>& vertices)
		:
		_stride(sizeof(T)) 
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(T)*vertices.size());
		bd.StructureByteStride = sizeof(T);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GFX_THROW_INFO(gfx.getDevice()->CreateBuffer(&bd, &sd, &_pVertexBuffer));

	}

	void bind(Graphics& gfx) noexcept override;
protected:
	UINT _stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pVertexBuffer;
};

