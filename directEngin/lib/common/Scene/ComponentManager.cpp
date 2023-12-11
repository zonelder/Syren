#include "ComponentManager.h"




ComponentManager::~ComponentManager()
{

	for (auto& [key, pool] : _pools)
	{
		delete pool;
	}
}
