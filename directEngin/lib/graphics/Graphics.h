#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

#include "exeption/pointed_exception.h"
#include "direct/DxgiInfoManager.h"

class Graphics
{
public:
	class Exeption :public PointedException {
		using PointedException::PointedException;
	};
	class HrException :public Exeption {
	public:
		HrException(int line, const char* file, HRESULT hr,std::vector<std::string> = {}) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		HRESULT getErrorCode() const noexcept;
		std::string getErrorString() const noexcept;
		std::string getErrorDescription() const noexcept;
		std::string getErrorInfo() const noexcept;
	private:
		HRESULT _hr;
		std::string _info;
	};
	class DeviceRemoveException :public HrException {
		using HrException::HrException;
	public:
		const char* getType() const noexcept override;
	private:
		std::string _reason;
	};
	class InfoException :public Exeption{
	public:
		InfoException(int line, const char* file,std::vector<std::string>);
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		std::string getErrorInfo() const noexcept;
	private:
		std::string _info;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	UINT screenWidth() const noexcept
	{
		return _width;
	}

	UINT screenHeight() const noexcept
	{
		return _height;
	}

	void endFrame();
	void clearBuffer(float red, float green, float blue) noexcept;
	void clearBuffer(const float color[4]) noexcept;
	void clearBuffer(DirectX::XMVECTOR color) noexcept;
	void drawIndexed(size_t indexCount, size_t startIndex =  0u);
	void draw(size_t indexCount, size_t startIndex = 0u);
	ID3D11DeviceContext* getContext() noexcept;
	ID3D11Device* getDevice() noexcept;
	DxgiInfoManager& getInfoManager() noexcept;

	void bindBackBuffer();
	auto pDepthBuffer() const noexcept
	{
		return _pDSV;
	}
private:
#ifndef NDEBUG
	DxgiInfoManager _infoManager;
#endif // !NDEBUG
	Microsoft::WRL::ComPtr<ID3D11Device> _pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> _pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _pDSV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _pDSState;

	UINT _width;
	UINT _height;
};

#endif