#include "Graphics.h"
#include "../direct/dxerr.h"
#include <sstream>
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException(__LINE__,__FILE__,(hr))
#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__,__FILE__,(hr),_infoManager.getMessage())
#define GFX_THROW_INFO(hrcall) _infoManager.set(); if(FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemoveException(__LINE__,__FILE__,(hr),_infoManager.getMessage())
#define GFX_THROW_INFO_ONLY(call) _infoManager.set();(call);{auto v = _infoManager.getMessage(); if(!v.empty()){throw Graphics::InfoException(__LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__,__FILE__,(hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemoveException(__LINE__,__FILE__,(hr))
#define GFX_THROW_INFO_ONLY(call) (call)
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

		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
		GFX_THROW_INFO(_pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		GFX_THROW_INFO(_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &_pTarget));

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

void Graphics::clearBuffer(float red, float green, float blue) noexcept{
	const float color[] = { red,green,blue,1.0f };
	_pContext->ClearRenderTargetView(_pTarget.Get(), color);
}

void Graphics::drawTestTriangle() {
	HRESULT hr;

	struct Vertex {
		float x;
		float y;
	};

	const Vertex vertices[] = {
		{ 0.0f,0.5f },
		{ 0.5f,-0.5f },
		{ -0.5f,-0.5f },
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	GFX_THROW_INFO(_pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	_pContext->IASetVertexBuffers(0u,1,&pVertexBuffer,&stride,&offset);

	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_INFO(_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	GFX_THROW_INFO_ONLY(_pContext->Draw((UINT)std::size(vertices), 0u));
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