#include "Graphics.h"
#include "../direct/dxerr.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "GraphicsThrowMacros.h"
#include "Drawable/BindComponent/BindableComponents.h"
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
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
		GFX_THROW_INFO(_pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

		_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

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
		GFX_THROW_INFO(_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

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

void Graphics::clearBuffer(float red, float green, float blue) noexcept{
	const float color[] = { red,green,blue,1.0f };
	_pContext->ClearRenderTargetView(_pTarget.Get(), color);
	_pContext->ClearDepthStencilView(_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::drawTestTriangle(float angle,float x,float y,float z) {

	HRESULT hr;

	const std::vector<DirectX::XMVECTOR> vertices = {
		{ -1.0f,-1.0f,-1.0f,},
		{ 1.0f,-1.0f,-1.0f, },
		{ -1.0f,1.0f,-1.0f, },
		{ 1.0f,1.0f,-1.0f,  },
		{ -1.0f,-1.0f,1.0f, },
		{ 1.0f,-1.0f,1.0f,  },
		{ -1.0f,1.0f,1.0f,  },
		{ 1.0f,1.0f,1.0f,   },
	};
	auto vertexBuffer = VertexBuffer(*this, vertices);
	vertexBuffer.bind(*this);

	//create index buffer
	const std::vector<unsigned short>indices = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	auto indexBuffer = IndexBuffer(*this, indices);
	indexBuffer.bind(*this);


	// create vertex constant buffer
	struct ConstartBuffer {
		DirectX::XMMATRIX transform;
	};
	const ConstartBuffer cb = {
		{
				DirectX::XMMatrixTranspose(
					DirectX::XMMatrixRotationZ(angle) *
					DirectX::XMMatrixRotationY(angle)*
					DirectX::XMMatrixTranslation(x,0,z+4.0f) *
					DirectX::XMMatrixPerspectiveFovLH(1.0f,3.0f / 4.0f,0.5f,10.0f)
				)
		}
	};
	auto vertexConstantBuffer = VertexConstantBuffer<ConstartBuffer>(*this, cb);
	vertexConstantBuffer.bind(*this);

	// create pixel constant buffer
	struct ConstantBuffer2 {
		struct {
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 = {
		{
			{1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
			{1.0f,1.0f,0.0f},
			{0.0f,1.0f,1.0f},
		}
	};
	auto pixelConstantBuffer = PixelConstantBuffer<ConstantBuffer2>(*this, cb2);
	pixelConstantBuffer.bind(*this);

	//create pixel shader
	auto pixelShader = PixelShader(*this, L"PixelShader.cso");
	pixelShader.bind(*this);

	auto vertexshader = VertexShader(*this, L"VertexShader.cso");
	vertexshader.bind(*this);
	ID3DBlob* pBlob = vertexshader.getBytecode();

	//unput vertex layout
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	auto inputLayout = InputLayout(*this, ied, pBlob);
	inputLayout.bind(*this);

	auto topology = Topology(*this, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	topology.bind(*this);


	GFX_THROW_INFO_ONLY(_pContext->DrawIndexed((UINT)std::size(indices),0u, 0u));
}

void Graphics::DrawIndexed(const std::vector<unsigned short>& indices)
{
	GFX_THROW_INFO_ONLY(_pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
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
