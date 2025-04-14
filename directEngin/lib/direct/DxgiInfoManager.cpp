#include "DxgiInfoManager.h"
#include "common/Window.h"
#include "graphics/Graphics.h"
#include "common/WindowThrowMacros.h"

#include <memory>
#include <dxgidebug.h>

#pragma comment(lib,"dxguid.lib")

#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__,__FILE__,hr)


DxgiInfoManager::DxgiInfoManager(){

	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr) {
		throw WND_LAST_EXCEPT();
	}

	const auto DxgiDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiDebugInterface == nullptr) {
		throw WND_LAST_EXCEPT();
	}
	HRESULT hr;
	GFX_THROW_NOINFO(DxgiDebugInterface(__uuidof(IDXGIInfoQueue),&_dxgiInfoQueue));
}



void DxgiInfoManager::set() noexcept{
	_next = _dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::getMessage() const {
	std::vector<std::string> messages;
	const auto end = _dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = _next; i < end; ++i) {
		HRESULT hr;
		SIZE_T messageLength;
		GFX_THROW_NOINFO(_dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL,i, nullptr, &messageLength));
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		GFX_THROW_NOINFO(_dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}
