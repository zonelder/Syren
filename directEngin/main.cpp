#include <Windows.h>
#include "WindowsMessageMap.h"
#include "KeyHandler.h"
#include <sstream>
#include "lib/vec2.h"
#include "Window.h"
#include "lib/exeption/PointedException.h"

InputHandler inputHandler;

int  CALLBACK WinMain(
	HINSTANCE hInstance,//
	HINSTANCE hPrevInstance,//always null
	LPSTR lpCmdLine,//command line
	int ncmdMode) {
	try {
		Window wnd(800, 300, "engin win");
		//message pump

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {

			//TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return (gResult == -1) ? (-1) : (msg.wParam);
	}
	catch (const PointedException& e) {

		MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {

		MessageBox(nullptr,e.what(),"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBox(nullptr, "no details available", "unknown Exception",MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;

}
