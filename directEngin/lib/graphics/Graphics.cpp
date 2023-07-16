#include "Graphics.h"
#include "../direct/dxerr.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "GraphicsThrowMacros.h"

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

	struct Vertex {
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};

	const Vertex vertices[] = {
		{ -1.0f,-1.0f,-1.0f,},
		{ 1.0f,-1.0f,-1.0f, },
		{ -1.0f,1.0f,-1.0f, },
		{ 1.0f,1.0f,-1.0f,  },
		{ -1.0f,-1.0f,1.0f, },
		{ 1.0f,-1.0f,1.0f,  },
		{ -1.0f,1.0f,1.0f,  },
		{ 1.0f,1.0f,1.0f,   },
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
	_pContext->IASetVertexBuffers(0u, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	//create index buffer
	const unsigned short indices[] = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	GFX_THROW_INFO(_pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	_pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


	// create constant buffer
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
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GFX_THROW_INFO(_pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	_pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

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

	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	GFX_THROW_INFO(_pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));
	_pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

	//create pixel shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GFX_THROW_INFO(_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	//create vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_INFO(_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	//bind shaders
	_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	//unput vertex layout(2d position only)
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	GFX_THROW_INFO(_pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));
	_pContext->IASetInputLayout(pInputLayout.Get());

	_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pContext->RSSetViewports(1u, &vp);

	GFX_THROW_INFO_ONLY(_pContext->DrawIndexed((UINT)std::size(indices),0u, 0u));
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
