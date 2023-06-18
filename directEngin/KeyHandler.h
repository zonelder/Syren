#pragma once
#include <windows.h>
#include <string>

enum MOUSE_KEY
{
	LEFT,
	RIGHT,
};

class InputHandler
{

public:
	void keyDown(WPARAM,LPARAM);
	void keyUp(WPARAM, LPARAM);

	void mouseDown(WPARAM, LPARAM,MOUSE_KEY);
	void mouseUp(WPARAM, LPARAM, MOUSE_KEY);
};

