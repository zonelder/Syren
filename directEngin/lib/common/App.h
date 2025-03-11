#pragma once
#include <unordered_map>
#include <vector>

#include "window.h"
#include "Time/Timer.h"
#include "Scene/scene_manager.h"
#include "Scene/system_manager.h"
#include "resmngr/resource_manager.h"

class App
{
public:
	App();
	int Init();
	void OnInit();
private:
	void Update();
	void Frame();
private:
	Window			_wnd;
	SceneContext	_context;

	SceneManager	_scene;
	SystemManager	_systemManager;
	Timer _time;//TODO change it to Time class after all

};

