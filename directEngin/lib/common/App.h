#pragma once
#include "Window.h"
#include "../Time/Timer.h"
#include "../graphics/Drawable/Box.h"
#include <vector>

class App
{
public:
	App();
	int Init();
private:
	void Update();
	void Frame();
private:
	Window _wnd;
	Graphics _gfx;
	Timer _time;//TODO change it to Time class after all
	Box _box1;
	Box _box2;

};

