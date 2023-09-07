#pragma once
#include "Window.h"
#include "../Time/Timer.h"
#include "../graphics/Drawable/Box.h"
#include <vector>

class App
{
public:
	App();
	int init();
private:
	void frame();
private:
	Window _wnd;
	Graphics _gfx;
	Timer _time;//TODO change it to Time class after all
	Box _box1;
	Box _box2;

};

