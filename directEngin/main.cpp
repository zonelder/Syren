#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <sstream>
#include "lib/common/app.h"
#include "lib/exeption/pointed_exception.h"
#include "lib//common/mouse_handler.h"

#define CONSOLE_ENABLE

InputHandler inputHandler;

int  CALLBACK WinMain(
	HINSTANCE hInstance,//
	HINSTANCE hPrevInstance,//always null
	LPSTR lpCmdLine,//command line
	int ncmdMode) {

#ifdef CONSOLE_ENABLE
	FILE* conout = stdout;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&conout,"CON", "w", stdout);
#endif
	try {
		return App{}.Init();
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
