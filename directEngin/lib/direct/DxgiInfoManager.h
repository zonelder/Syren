#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <wrl.h>//interfaces inside directX alredy cointing in COM model if we try to use shared_poiter instead we catch unlocated memory
#include <dxgidebug.h>


class DxgiInfoManager{

public:
	DxgiInfoManager();
	~DxgiInfoManager() = default;
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	void set() noexcept;
	std::vector<std::string> getMessage() const;
private:
	unsigned long long _next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> _dxgiInfoQueue;
};

