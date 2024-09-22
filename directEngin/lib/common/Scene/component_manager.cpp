#include "component_manager.h"


ComponentManager::~ComponentManager()
{

	for (auto& [key, pool] : _pools)
	{
		delete pool;
	}
}

