#pragma once
#include "Window.h"
#include "../Time/Timer.h"
#include "../graphics/Drawable/Primitive.h"
#include "Scene/Camera/Camera.h"
#include <vector>
#include "../Systems/RenderSystem.h"
#include "../Systems/OrientationSystem.h"

class App
{
public:
	App();
	int Init();
private:
	void Update();
	void Frame();

	/// @brief Update static Mouse data  
	void SetInputData();
private:
	Window _wnd;
	Graphics _gfx;
	RenderSystem _renderSystem;
	OrientationSystem _orientationSystem;
	Camera _mainCamera;
	Timer _time;//TODO change it to Time class after all
	std::vector<Primitive> _obj;

};

