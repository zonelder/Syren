#include "pch.h"
#include "entity_manager.h"


Entity& EntityManager::create() noexcept
{
	EntityID id = 0;
	if (!_gaps.empty())
	{
		id = _gaps.back();
		_gaps.pop_back();
	}
	else
	{
		id = ++_max;
	}
	auto& entt = _entities.add(id);
	entt._ID = id;//TODO better rebuld "add" in template constructor. no need to use friend injection
	return entt;
}



bool EntityManager::destroy(const Entity& entt) noexcept
{
	const  auto id = entt.getID();
	if (!_entities.contains(id))
		return false;

	auto res = _entities.remove(id);
	if (!res)
		return false;

	if (id == _max)
	{
		--_max;
	}
	else
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
	return _entities.contains(id);
}

void EntityManager::registerComponent(EntityID entt,ComponentID id) noexcept
{
	_entities[entt].registerComponent(id);
}

void EntityManager::unregisterComponent(EntityID entt,ComponentID id) noexcept
{
	_entities[entt].unregisterComponent(id);
}
 