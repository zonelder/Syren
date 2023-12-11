#include "SystemManager.h"



void SystemManager::update(SceneManager& scene, float dt)
{

	for (auto& [prior,system] : _systems)
	{
		system->onUpdate(scene, dt);
	}
}

void SystemManager::frame(SceneManager& scene)
{

	for (auto& [priority,system] :_systems)
	{
		system->onFrame(scene);
	}
}


void SystemManager::removeSystem(unsigned int priority) noexcept
{
	if (!_systems.contains(priority))
	{
		//TODO add warn: attempt to remove system with priority = $priority, but there is not such system in the list  
		return;
	}

	_systems.erase(priority);
}

void SystemManager::resetPriority(unsigned int curPriority, unsigned int priority) noexcept
{
	if (!_systems.contains(curPriority))
	{
		//TODO add warn: attempt to swap priority of system with priority = curPriority, but there is not such system in the list  
		return;
	}

	auto system_pair = _systems.extract(curPriority);
	system_pair.key() = priority;
	_systems.insert(std::move(system_pair));
}