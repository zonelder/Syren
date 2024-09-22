#include "index_buffer.h"


IndexBuffer::IndexBuffer(Graphics& gfx,const std::vector<unsigned short>& indices)
	:
	p_count((UINT)indices.size())
{
	INFOMAN(gfx);
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(p_count*sizeof(unsigned short));
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	GFX_THROW_INFO(gfx.getDevice()->CreateBuffer(&ibd, &isd, &p_pIndexBuffer));
}


void IndexBuffer::bind(Graphics& gfx) noexcept
{
	gfx.getContext()->IASetIndexBuffer(p_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::getCount() const noexcept
{
	return p_count;
}
