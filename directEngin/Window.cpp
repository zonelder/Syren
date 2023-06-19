#include "Window.h"
#include <sstream>
#include "resource.h"
#include "WindowsMessageMap.h"


Window::WindowClass::WindowClass() noexcept
	:_hInst(GetModuleHandle(nullptr)){
	//register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = handleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(getInstance(), MAKEINTRESOURCE(IDB_PNG1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName();
	wc.hIcon = static_cast<HICON>(LoadImage(getInstance(), MAKEINTRESOURCE(IDB_PNG1), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}
Window::WindowClass Window::WindowClass::_wndClass;

Window::WindowClass::~WindowClass() {
	UnregisterClass(_wndCLassName, getInstance());
}

const char* Window::WindowClass::getName() noexcept {
	return _wndCLassName;
}

HINSTANCE Window::WindowClass::getInstance() noexcept {
	return _wndClass._hInst;
}

Window::Window(int width, int height, const char* name) noexcept {

	RECT wr{ 0 };
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (FAILED(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))) {
		throw WND_LAST_EXCEPT();
	}
	_hWnd = CreateWindow(WindowClass::getName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::getInstance(), this
	);
	
	if (_hWnd == nullptr) {
		throw WND_LAST_EXCEPT();
	}
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {

	DestroyWindow(_hWnd);
}

LRESULT WINAPI Window::handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if (msg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleMsgThunk));

		return pWnd->handleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return pWnd->handleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	switch (msg)
	{
	case WM_CLOSE:	
		PostQuitMessage(0);
		 break;

	case WM_KILLFOCUS:
		inputHandler.clearState();
		break;
	//< Input message >
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || inputHandler.isEnabledAutorepeat()) {//filter in case user hold button
			inputHandler.onkeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYDOWN:
		// TODO change key states to Down\up\pressed\unvalid to procces the hold-key case 
		if (!(lParam & 0x40000000) || inputHandler.isEnabledAutorepeat()) {//filter in case user hold button
			inputHandler.onkeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_SYSKEYUP:
		inputHandler.onKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
		inputHandler.onKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		inputHandler.onChar(static_cast<unsigned char>(wParam));
		break;
	//< /Input message >
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}



///Window::Exeption stuff

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	PointedException(line, file), _hr(hr)
{}

const char* Window::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[ERROR CODE]" << getErrorCode() << std::endl
		<< "[DESCRIPTION]" << getErrorString() << std::endl
		<< getOriginalString();
	p_buffer = oss.str();
	return p_buffer.c_str();
}

const char* Window::Exception::getType() const noexcept {

	return "Windows Exception";
}

std::string Window::Exception::translateErrorCode(HRESULT hr) noexcept{

	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

	if (nMsgLen == 0) {
		return "Unidentified error code";
	}

	std::string errorStr = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorStr;
}

HRESULT  Window::Exception::getErrorCode() const noexcept {

	return _hr;
}

std::string  Window::Exception::getErrorString() const noexcept {

	return translateErrorCode(_hr);
}


