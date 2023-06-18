#include "Window.h"


Window::WindowClass::WindowClass()noexcept
	:_hInst(GetModuleHandle(nullptr)){
	//register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = handleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName();
	wc.hIcon = nullptr;
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
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	_hWnd = CreateWindow(WindowClass::getName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::getInstance(), this
	);
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
	case WIM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
