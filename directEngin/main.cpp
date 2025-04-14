#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <sstream>
#include "common/app.h"
#include "exeption/pointed_exception.h"

#define CONSOLE_ENABLE

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
		return App{}.init();
	}
	catch (const PointedException& e) {

		MessageBoxA(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {

		MessageBoxA(nullptr,e.what(),"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {

		MessageBoxA(nullptr, "no details available", "unknown Exception",MB_OK | MB_ICONEXCLAMATION);
	}



	
	return -1;

}
