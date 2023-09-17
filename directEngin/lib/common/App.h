#pragma once
#include "Window.h"
#include "../Time/Timer.h"
#include <vector>
#include "Scene/SceneManager.h"
#include "Scene/ComponentManager.h"
#include <unordered_map>


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
	SceneManager _sceneManager;
	Timer _time;//TODO change it to Time class after all

};

