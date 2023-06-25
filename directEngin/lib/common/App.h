#pragma once
#include "Window.h"
#include "../Time/Timer.h"
class App
{
public:
	App();
	int init();
private:
	void update();
private:
	Window _wnd;
	Timer _time;//TODO change it to Time class after all
};

