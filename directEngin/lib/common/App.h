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
	/// @brief Main Draw function
	void frame();
	void Update(float dt = 0.0f);
private:
	Window _wnd;
	Timer _time;//TODO change it to Time class after all
	Box _box1;
	Box _box2;

};

