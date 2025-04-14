#include "pch.h"
#include "geometric_buffer.h"

GBuffer::GBuffer(Graphics* pGfx)
{
   //auto pDevice = pGfx->getDevice();
    auto width = pGfx->screenWidth();
    auto height = pGfx->screenHeight();
    _pAlbedo = std::make_unique<RenderTarget>(pGfx, width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
    _pNormal = std::make_unique<RenderTarget>(pGfx, width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);
    _pDepth = std::make_unique<RenderTarget>(pGfx, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
}


void GBuffer::bind(Graphics* pGfx)
{
    ID3D11RenderTargetView* rtvs[] = {
        _pAlbedo->GetRTV(),
        _pNormal->GetRTV(),
        _pDepth->GetRTV()
    };
    pGfx->getContext()->OMSetRenderTargets(3, rtvs, pGfx->pDepthBuffer().Get());
}

void GBuffer::unbind(Graphics* pGfx)
{
    ID3D11RenderTargetView* nullRTVs[] = { nullptr, nullptr, nullptr };
    pGfx->getContext()->OMSetRenderTargets(3, nullRTVs,nullptr);
}


void GBuffer::clear(Graphics* pGfx,const float color[4])
{
    _pAlbedo->clear(pGfx, color);
    _pNormal->clear(pGfx, color);
    _pDepth->clear(pGfx, color);
}