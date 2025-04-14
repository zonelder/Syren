#ifndef __WINDOW_H__
#define __WINDOW_H__

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "..\exeption\pointed_exception.h"
#include "..\graphics\graphics.h"
#include "input_handler.h"
#include "mouse_handler.h"

#include <windows.h>

#undef max
#undef min


class Window
{

public:

	struct ProcessResult
	{
		bool hasExit{ false };
		int exitCode{ 0 };

		operator bool() const noexcept { return hasExit; };
	};

	class Exception :public PointedException {
		using PointedException::PointedException;
	public:
		static std::string translateCode(HRESULT hr) noexcept;
	};
	class HrException:public Exception{
	public:
		HrException(int, const char* file, HRESULT) noexcept;
		const char* what() const noexcept override;
		virtual const char* getType() const noexcept;
		static std::string translateErrorCode(HRESULT hr) noexcept;
		HRESULT getErrorCode() const noexcept;
		std::string getErrorDscription() const noexcept;
	private:
		HRESULT _hr;
	};
	class NoGfxException :public Exception {
	public:
		using Exception::Exception;
		const char* getType() const noexcept override;
	};
private:
	/// @brief singleton manager registration\cleanup of window class
	class WindowClass
	{
	public:
		static const wchar_t* getName() noexcept;
		static HINSTANCE getInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* _wndCLassName = L"syrenEnginWindow";
		static WindowClass _wndClass;
		HINSTANCE _hInst;

	};
public:
	Window(int width, int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetTitle(const std::string&);
	static ProcessResult processMessage();
	HWND GetHWND() const noexcept;


	int GetWidth() const noexcept;
	int GetHeight() const noexcept;
private:
	static LRESULT CALLBACK handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lPram);
	static LRESULT CALLBACK handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lPram);
	LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	InputHandler inputHandler;
	MouseHandler mouseHandler;
private:
	int _width;
	int _height;
	HWND _hWnd;
};




#endif