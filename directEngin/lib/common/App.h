#pragma once
#include "Window.h"
#include "../Time/Timer.h"
#include <vector>
#include "Scene/scene_manager.h"
#include "Scene/component_manager.h"
#include <unordered_map>
#include "Scene/system_manager.h"


class App
{
public:
	App();
	int Init();
	void OnInit();
private:
	void Update();
	void Frame();

	/// @brief Update static Mouse data  
	void SetInputData();
private:
	Window _wnd;
	SceneManager _scene;
	SystemManager _systemManager{};
	Timer _time;//TODO change it to Time class after all

};

