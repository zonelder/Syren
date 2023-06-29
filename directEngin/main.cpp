#include <Windows.h>
#include <sstream>
#include "lib/vec2.h"
#include "lib/common/App.h"
#include "lib/exeption/PointedException.h"
#include "lib//common/MouseHandler.h"

InputHandler inputHandler;

int  CALLBACK WinMain(
	HINSTANCE hInstance,//
	HINSTANCE hPrevInstance,//always null
	LPSTR lpCmdLine,//command line
	int ncmdMode) {
	try {
		return App{}.init();
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
