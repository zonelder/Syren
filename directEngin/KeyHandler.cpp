#include "KeyHandler.h"
#include "lib/vec2.h"


void InputHandler::keyDown(WPARAM wParam,LPARAM lParam){

	char name[256];
	GetKeyNameTextA(lParam, name, 256);
	std::string  button_log = std::string(name) +" button pressed\n";
	OutputDebugString(button_log.c_str());
}

void InputHandler::keyUp(WPARAM wParam, LPARAM lParam) {

	char name[256];
	GetKeyNameTextA(lParam, name, 256);
	std::string  button_log = std::string(name) + " button up\n";
	OutputDebugString(button_log.c_str());
}

void InputHandler::mouseDown(WPARAM wParam, LPARAM lParam,MOUSE_KEY key) {
	const POINTS pt = MAKEPOINTS(lParam);
}
