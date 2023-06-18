#pragma once
#include <windows.h>
#include "lib\exeption\PointedException.h"


class Window
{

public:
	class Exception :public PointedException {
	public:
		Exception(int, const char* file, HRESULT) noexcept;
		const char* what() const noexcept override;
		virtual const char* getType() const noexcept;
		static std::string translateErrorCode(HRESULT hr) noexcept;
		HRESULT getErrorCode() const noexcept;
		std::string getErrorString() const noexcept;
	private:
		HRESULT _hr;
	};
private:
	/// @brief singleton manager registration\cleanup of window class
	class WindowClass
	{
	public:
		static const char* getName() noexcept;
		static HINSTANCE getInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* _wndCLassName = "syrenEnginWindow";
		static WindowClass _wndClass;
		HINSTANCE _hInst;

	};
public:
	Window(int width, int height, const char* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lPram);
	static LRESULT CALLBACK handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lPram);
	LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	int _width;
	int _height;
	HWND _hWnd;
};

#define WND_EXCEPTION(hr) Window::Exception( __LINE__,__FILE__,hr )
#define WND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )


