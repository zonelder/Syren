#pragma once
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void endFrame();
	void clearBuffer(float red, float green, float blue) noexcept;
private:
	ID3D11Device* _pDevice = nullptr;
	IDXGISwapChain* _pSwap = nullptr;
	ID3D11DeviceContext* _pContext = nullptr;
	ID3D11RenderTargetView* _pTarget = nullptr;
};

