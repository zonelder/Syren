#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__

#include <wrl/client.h>
#include <dxgiformat.h>
#include "graphics/Graphics.h"
#include <memory>

using Microsoft::WRL::ComPtr;

class RenderTarget
{
public:
	RenderTarget(Graphics* pGfx, size_t width, size_t height, DXGI_FORMAT  format);

	void bind(Graphics* pGfx);
	void clear(Graphics* pGfx,const float color[4]);

    ID3D11RenderTargetView* GetRTV() const { return _pRTV.Get(); }
    ID3D11ShaderResourceView* GetSRV() const { return _pSRV.Get(); }
private:
    UINT _width, _height;
    DXGI_FORMAT _format;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> _pTexture;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pRTV;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pSRV;
};



#endif
