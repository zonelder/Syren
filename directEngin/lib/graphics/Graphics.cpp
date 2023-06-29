#include "Graphics.h"
#include "../direct/dxerr.h"
#include <sstream>
#pragma comment(lib,"d3d11.lib")

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException(__LINE__,__FILE__,(hr))
#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__,__FILE__,(hr),_infoManager.getMessage())
#define GFX_THROW_INFO(hrcall) _infoManager.set(); if(FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemoveException(__LINE__,__FILE__,(hr),_infoManager.getMessage())
#else
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__,__FILE__,(hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemoveException(__LINE__,__FILE__,(hr))
#endif // !NDEBUG

Graphics::Graphics(HWND hWnd){
	
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlag = 0u;
#ifndef NDEBUG
	swapCreateFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !NDEBUG

	//for GFX_THROW_FAILED macro
	HRESULT hr;
	//create device and front\back buffer
	try {

		GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlag,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&_pSwap,
			&_pDevice,
			nullptr,
			&_pContext
		));

		ID3D11Resource* pBackBuffer = nullptr;
		GFX_THROW_INFO(_pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));
		GFX_THROW_INFO(_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pTarget));
		pBackBuffer->Release();

	}
	catch (const PointedException& e) {
		MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
	}
}

Graphics::~Graphics(){
	
	if (_pDevice != nullptr) {
		_pDevice->Release();
	}
	if (_pSwap != nullptr) {
		_pSwap->Release();
	}
	if (_pContext != nullptr) {
		_pContext->Release();
	}
	if (_pTarget != nullptr) {
		_pTarget->Release();
	}

}

void Graphics::endFrame(){

	HRESULT hr;
	if (FAILED(hr = _pSwap->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			throw GFX_DEVICE_REMOVED_EXCEPT(_pDevice->GetDeviceRemovedReason());
		}
		else {
			GFX_EXCEPT(hr);
		}
	}
}

void Graphics::clearBuffer(float red, float green, float blue) noexcept{
	const float color[] = { red,green,blue,1.0f };
	_pContext->ClearRenderTargetView(_pTarget, color);
}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr,std::vector<std::string> infoMsgs) noexcept:
	Exeption(line,file),
	_hr(hr)
{
	for (const auto& m : infoMsgs) {
		_info += m;
		_info.push_back('\n');
	}
	if (!_info.empty()) {
		_info.pop_back();
	}
}
std::string Graphics::HrException::getErrorInfo() const noexcept
{
	return _info;
}


const char* Graphics::HrException::what() const noexcept {

	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[Errr Code] 0x" << std::hex << std::uppercase << getErrorCode()
		<< std::dec << " (" << (unsigned long)getErrorCode() << ")" << std::endl
		<< "[Error String]" << getErrorString() << std::endl
		<< "[Description]" << getErrorDescription() << std::endl;
	if (!_info.empty()) {
		oss << "\n[Error Info]\n" << getErrorInfo() << std::endl;
	}
	oss << getOriginalString();
	p_buffer = oss.str();

	return p_buffer.c_str();
}
const char* Graphics::HrException::getType() const noexcept {

	return "Graphic Exception";
}

HRESULT  Graphics::HrException::getErrorCode() const noexcept {

	return _hr;
}

std::string  Graphics::HrException::getErrorString() const noexcept{

	return DXGetErrorString(_hr);
}

std::string  Graphics::HrException::getErrorDescription() const noexcept {

	char buf[512];
	DXGetErrorDescription(_hr, buf, sizeof(buf));
	return buf;
}

const char* Graphics::DeviceRemoveException::getType() const noexcept {

	return "Graphic Exception[ Device Removed ](DXGI_ERROR_DEVICE_REMOVED)";
}

