#pragma once
#include "Window.h"
#include "../Time/Timer.h"
#include "../graphics/Drawable/Primitive.h"
#include "Scene/Camera/Camera.h"
#include <vector>
#include "../Systems/RenderSystem.h"
#include "../Systems/OrientationSystem.h"
#include <unordered_map>

using EntityID = unsigned int;

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
	std::unordered_map<EntityID, Transform> _transforms;
	std::unordered_map<EntityID, Render> _renders;
	RenderSystem _renderSystem;
	OrientationSystem _orientationSystem;
	Camera _mainCamera;
	Timer _time;//TODO change it to Time class after all

};

