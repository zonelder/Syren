#ifndef __APP_H__
#define __APP_H__

#include "core/window.h"
#include "Time/Timer.h"
#include "Scene/scene_manager.h"
#include "Scene/system_manager.h"
#include "resmgr/resource_manager.h"
#include "resmgr/scene_context.h"

#include <unordered_map>

std::unordered_map<int, EntityID>  instantiate(SceneManager& scene, FbxPrefabPtr pPrefab);

class App
{
public:
	App();
	int init();
	void onInit();
private:
	void update();
	void frame();
private:
	Window			_wnd;
	SceneContext	_context;

	SceneManager	_scene;
	SystemManager	_systemManager;
	Timer _time;//TODO change it to Time class after all

};

#endif

