#ifndef __DYNAMIC_BUFFER_H__
#define __DYNAMIC_BUFFER_H__
#include "IBindable.h"


class DynamicBuffer 
{

public:
    DynamicBuffer(Graphics& gfx, UINT elementSize, UINT initialCapacity = 1024);

    void update(const void* data, UINT elementCount, UINT elementSize);

    ID3D11Buffer* get() const { return _pBuffer.Get(); }

private:
    void resize(UINT elementSize, UINT newCapacity);

    Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;
    UINT _capacity;
    Graphics* _pGfx;
};

#endif