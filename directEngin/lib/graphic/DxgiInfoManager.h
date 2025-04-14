#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <wrl.h>
#include <dxgidebug.h>

#include <vector>
#include <string>

class DxgiInfoManager
{

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

