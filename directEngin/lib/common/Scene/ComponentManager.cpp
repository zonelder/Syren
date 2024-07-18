#include "ComponentManager.h"


ComponentManager::~ComponentManager()
{

	for (auto& [key, pool] : _pools)
	{
		delete pool;
	}
}



void ComponentManager::removeComponent(ComponentID comp_id, EntityID entt_id)
{
	_pools[comp_id]->removeComponent(entt_id);
}