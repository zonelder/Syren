#include "Window.h"
#include <sstream>
#include "WindowThrowMacros.h"

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

Window::Window(int width, int height, const char* name) noexcept 
	:
	_width(width),
	_height(height){

	RECT wr{ 0 };
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0 ) {
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

	_pGraphic = std::make_unique<Graphics>(_hWnd);
}

Window::~Window() {

	DestroyWindow(_hWnd);
}

void Window::SetTitle(const std::string& title)  {

	if (SetWindowText(_hWnd, title.c_str()) == 0) {
		throw WND_LAST_EXCEPT();
	}
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

	//< Mouse message>
	case WM_MOUSEMOVE: {
		const POINTS pt = MAKEPOINTS(lParam);
		//if client hold mouse in a window
		if (pt.x >= 0 && pt.x < _width && pt.y >= 0 && pt.y < _height) {
			mouseHandler.onMouseMove(pt.x, pt.y);

			if (!mouseHandler.isInWindow()) {
				SetCapture(_hWnd);
				mouseHandler.onMouseEnter();
			}
		}
		else {
			//outside of a client region
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {// if that tthen clien produce a drag operation
				mouseHandler.onMouseMove(pt.x, pt.y);
			}
			else {
				ReleaseCapture();
				mouseHandler.onMouseLeave();
			}
		}

		break;
	}
	case WM_LBUTTONDOWN: {
		const POINTS pt = MAKEPOINTS(lParam);
		mouseHandler.onLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP: {
		const POINTS pt = MAKEPOINTS(lParam);
		mouseHandler.onLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN: {
		const POINTS pt = MAKEPOINTS(lParam);
		mouseHandler.onRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP: {
		const POINTS pt = MAKEPOINTS(lParam);
		mouseHandler.onRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL: {
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouseHandler.onWheelDelta(pt.x, pt.y, delta);
		break;
	}
	//< /MouseMessage>


	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}



///Window::Exeption stuff

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file), _hr(hr)
{}

const char* Window::HrException::what() const noexcept {
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[ERROR CODE]" << getErrorCode() << std::endl
		<< "[DESCRIPTION]" << getErrorDscription() << std::endl
		<< getOriginalString();
	p_buffer = oss.str();
	return p_buffer.c_str();
}

const char* Window::HrException::getType() const noexcept {

	return "Windows Exception";
}

std::string Window::HrException::translateErrorCode(HRESULT hr) noexcept{

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

HRESULT  Window::HrException::getErrorCode() const noexcept {

	return _hr;
}

std::string  Window::HrException::getErrorDscription() const noexcept {

	return translateErrorCode(_hr);
}

const char*  Window::NoGfxException::getType() const noexcept {

	return "Window exception [No Graphics]";
}

std::optional<int> Window::processMessage() {
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

		if (msg.message == WM_QUIT) {
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

Graphics& Window::getGraphic()
{
	if (!_pGraphic) {
		throw WND_NOGFX_EXCEPT();
	}
	return *_pGraphic;
}
