#pragma once
#include <Windows.h>
#include <vector>
#include <string>
class DxgiInfoManager{

public:
	DxgiInfoManager();
	~DxgiInfoManager();
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	void set() noexcept;
	std::vector<std::string> getMessage() const;
private:
	unsigned long long _next = 0u;
	struct IDXGIInfoQueue* _dxgiInfoQueue = nullptr;
};

