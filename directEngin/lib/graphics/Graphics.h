#pragma once
#include <d3d11.h>
#include "../exeption/PointedException.h"
#include <vector>
#include "../direct/DxgiInfoManager.h"
#include <wrl.h>//interfaces inside directX alredy cointing in COM model if we try to use shared_poiter instead we catch unlocated memory


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

	void endFrame();
	void clearBuffer(float red, float green, float blue) noexcept;
	void drawTestTriangle(float angle);
private:
#ifndef NDEBUG
	DxgiInfoManager _infoManager;
#endif // !NDEBUG
	Microsoft::WRL::ComPtr<ID3D11Device> _pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> _pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pTarget;
};

