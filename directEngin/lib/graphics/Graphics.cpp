#include "graphics.h"
#include "../direct/dxerr.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "graphics_throw_macros.h"
#include "Drawable/BindComponent/bindable_components.h"
#include <vector>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")



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

		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
		GFX_THROW_INFO(_pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		GFX_THROW_INFO(_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &_pTarget));


		// create depth stennsil state
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		GFX_THROW_INFO(_pDevice->CreateDepthStencilState(&dsDesc, &_pDSState));


		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;

		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = 800u;
		descDepth.Height = 600u;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;

		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		GFX_THROW_INFO(_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));
		/*
		Microsoft::WRL::ComPtr <ID3D11BlendState> blend;
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
		bd.AlphaToCoverageEnable = TRUE;
		auto& rt = bd.RenderTarget[0];
		rt.BlendEnable = TRUE;
		rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rt.BlendOp = D3D11_BLEND_OP_ADD;
		rt.SrcBlendAlpha = D3D11_BLEND_ZERO;
		rt.DestBlendAlpha = D3D11_BLEND_ZERO;
		rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rt.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
		GFX_THROW_INFO(_pDevice->CreateBlendState(&bd, &blend));

		UINT mask = 0xffffffff;
		_pContext->OMSetBlendState(blend.Get(), 0, mask);
		*/
		// create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;
		
		GFX_THROW_INFO(_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &_pDSV));

		//bind depth stensil view to OM
		_pContext->OMSetRenderTargets(1u, _pTarget.GetAddressOf(), _pDSV.Get());

		D3D11_VIEWPORT vp;
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		_pContext->RSSetViewports(1u, &vp);

	}
	catch (const PointedException& e) {
		MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
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

void Graphics::ClearBuffer(float red, float green, float blue) noexcept{
	const float color[] = { red,green,blue,1.0f };
	ClearBuffer(color);
}

void Graphics::ClearBuffer(const float color[4]) noexcept
{
	_pContext->ClearRenderTargetView(_pTarget.Get(), color);
	_pContext->ClearDepthStencilView(_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);

	_pContext->OMSetDepthStencilState(_pDSState.Get(), 0);

}
void Graphics::DrawIndexed(size_t indexCount,size_t startIndex)
{
	GFX_THROW_INFO_ONLY(_pContext->DrawIndexed((UINT)indexCount, (UINT)startIndex, 0u));
}

void Graphics::Draw(size_t indexCount, size_t startIndex)
{
	_pContext->Draw((UINT)indexCount, (UINT)startIndex);
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

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs)
	:
	Exeption(line, file) {
	for (const auto& m : infoMsgs) {
		_info += m;
		_info.push_back('\n');
	}
	if (!_info.empty()) {
		_info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept {
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "\n[Error Info]\n" << getErrorInfo() << std::endl;
	oss << getOriginalString();
	p_buffer = oss.str();
	return p_buffer.c_str();
	
}

const char* Graphics::InfoException::getType() const noexcept {
	return "Info Exception";
}


std::string Graphics::InfoException::getErrorInfo() const noexcept {
	return _info;
}

ID3D11DeviceContext* Graphics::getContext() noexcept{
	return _pContext.Get();
}

ID3D11Device* Graphics::getDevice() noexcept {
	return _pDevice.Get();
}
DxgiInfoManager& Graphics::getInfoManager() noexcept {
	return _infoManager;
}
