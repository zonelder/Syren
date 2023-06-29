#pragma once
#include <d3d11.h>
#include "../exeption/PointedException.h"
#include <vector>
#include "../direct/DxgiInfoManager.h"
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
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void endFrame();
	void clearBuffer(float red, float green, float blue) noexcept;
private:
#ifndef NDEBUG
	DxgiInfoManager _infoManager;
#endif // !NDEBUG
	ID3D11Device* _pDevice = nullptr;
	IDXGISwapChain* _pSwap = nullptr;
	ID3D11DeviceContext* _pContext = nullptr;
	ID3D11RenderTargetView* _pTarget = nullptr;
};

