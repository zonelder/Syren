#include "dynamic_buffer.h"

DynamicBuffer::DynamicBuffer(Graphics& gfx, UINT elementSize, UINT initialCapacity)
    : _pGfx(&gfx)
{
    resize(elementSize, initialCapacity);
}

void DynamicBuffer::update(const void* data, UINT elementCount, UINT elementSize) {
    // Если не хватает места - увеличиваем буфер в 1.5 раза
    if (elementCount > _capacity) {
        UINT newCapacity = static_cast<UINT>(elementCount * 1.5);
        resize(elementSize, newCapacity);
    }
    INFOMAN((*_pGfx));
    D3D11_MAPPED_SUBRESOURCE mapped;
    GFX_THROW_INFO(_pGfx->getContext()->Map(
        _pBuffer.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped
    ));

    memcpy(mapped.pData, data, elementCount * elementSize);
    _pGfx->getContext()->Unmap(_pBuffer.Get(), 0);
}

void DynamicBuffer::resize(UINT elementSize, UINT newCapacity) 
{
    INFOMAN((*_pGfx));
    D3D11_BUFFER_DESC desc = {};
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = elementSize * newCapacity;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    GFX_THROW_INFO(_pGfx->getDevice()->CreateBuffer(
        &desc,
        nullptr,
        &_pBuffer
    ));

    _capacity = newCapacity;
}
