#include "SystemManager.h"



void SystemManager::update(SceneManager& scene, float dt)
{

	for (auto& system : _systems)
	{
		system->onUpdate(scene, dt);
	}
}

void SystemManager::frame(SceneManager& scene)
{

	for (auto& system : _systems)
	{
		system->onFrame(scene);
	}
}
