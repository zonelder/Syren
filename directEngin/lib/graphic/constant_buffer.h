#pragma once
#include "IBindable.h"

template<typename T>
class ConstantBuffer:public IBindable
{
public:
	ConstantBuffer() = default;
	ConstantBuffer(Graphics& gfx, const T& consts)
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GFX_THROW_INFO(gfx.getDevice()->CreateBuffer(&cbd, &csd, &p_pConstantBuffer));
	}
	ConstantBuffer(Graphics& gfx)
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T);
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO(gfx.getDevice()->CreateBuffer(&cbd, nullptr, &p_pConstantBuffer));
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> p_pConstantBuffer = nullptr;
};


template<typename T> 
class VertexConstantBuffer :public ConstantBuffer<T>
{
public:
	using ConstantBuffer<T>::p_pConstantBuffer;

	using ConstantBuffer<T>::ConstantBuffer;
	void bind(Graphics& gfx) noexcept override
	{
		gfx.getContext()->VSSetConstantBuffers(0u, 1u, p_pConstantBuffer.GetAddressOf());
	}
};


template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::p_pConstantBuffer;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void bind(Graphics& gfx) noexcept override
	{
		gfx.getContext()->PSSetConstantBuffers(0u, 1u, p_pConstantBuffer.GetAddressOf());
	}
};
