#include "EntityManager.h"


Entity& EntityManager::create() noexcept
{
	EntityID id = 0;
	if (!_gaps.empty())
	{
		id = _gaps.back();
		_gaps.pop_back();
		_entities[id] = Entity(id);
	}
	else
	{
		id = ++_max;
		_entities.emplace_back(id);
	}


	return _entities[id];
}



bool EntityManager::destroy(const Entity& entt) noexcept
{
	auto id = entt.getID();
	if (_entities[id].getID() == -1) return false;
	_entities[id] = Entity(-1);

	if (id == _max)
		_max--;

	if (id < _max)
	{
		_gaps.push_back(id);
	}

	return true;
}

const Entity& EntityManager::get(EntityID id) const noexcept
{
	return _entities[id];
}

bool EntityManager::hasEntity(EntityID id) const noexcept
{
	return _entities[id].getID() != -1;
}

void EntityManager::registerComponent(EntityID entt,ComponentID id) noexcept
{
	_entities[entt].registerComponent(id);
}

void EntityManager::unregisterComponent(EntityID entt,ComponentID id) noexcept
{
	_entities[entt].unregisterComponent(id);
}
 