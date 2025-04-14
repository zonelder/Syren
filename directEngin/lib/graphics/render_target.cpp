#include "render_target.h"
#include <stdexcept>

RenderTarget::RenderTarget(Graphics * pGfx, size_t width, size_t height, DXGI_FORMAT format)
	:
	_width(width),
	_height(height),
	_format(format)
{
    auto pDevice = pGfx->getDevice();
    // Создаем текстуру
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = format;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;

    HRESULT hr = pDevice->CreateTexture2D(&texDesc, nullptr, _pTexture.GetAddressOf());
    if (FAILED(hr))
        throw std::runtime_error("Failed to create texture for render target");

    // Создаем RTV
    hr = pDevice->CreateRenderTargetView(_pTexture.Get(), nullptr, _pRTV.GetAddressOf());
    if (FAILED(hr))
        throw std::runtime_error("Failed to create render target view");

    // Создаем SRV
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    hr = pDevice->CreateShaderResourceView(_pTexture.Get(), &srvDesc, _pSRV.GetAddressOf());
    if (FAILED(hr))
        throw std::runtime_error("Failed to create shader resource view");
}

void RenderTarget::bind(Graphics* pGfx)
{
    pGfx->getContext()->OMSetRenderTargets(1, _pRTV.GetAddressOf(), pGfx->pDepthBuffer().Get());

}

void RenderTarget::clear(Graphics* pGfx,const float color[4])
{
    pGfx->getContext()->ClearRenderTargetView(_pRTV.Get(), color);
}
