#include "pch.h"
#include "Entity.h"

Entity::Entity(EntityID id):_ID(id){}

Entity::Entity():Entity(-1) {};

void Entity::registerComponent(ComponentID id) noexcept
{
	_components.set(id, true);
}

void Entity::unregisterComponent(ComponentID id) noexcept
{
	_components.set(id, false);
}

const EntityID Entity::getID() const noexcept
{
	return _ID;
}

Entity& Entity::operator=(const Entity& entt)
{
	_ID = entt._ID;
	_components = entt._components;
	return *this;
}


Entity::ComponentIterator::ComponentIterator(const Entity* comps):_comps(&(comps->_components)),index(-1){}

Entity::ComponentIterator::ComponentIterator(const Entity* comps, ComponentID start) :Entity::ComponentIterator(comps)
{
	index = start;
}

bool Entity::ComponentIterator::operator!=(const ComponentIterator& other) const
{
	return !(*this == other);
}

bool Entity::ComponentIterator::operator==(const ComponentIterator& other) const
{
	return _comps == other._comps && index == other.index;
}

ComponentID Entity::ComponentIterator::operator*() const
{
	return index;
}

Entity::ComponentIterator& Entity::ComponentIterator::operator++()
{
	++index;
	while (index < _comps->size() && !_comps->operator[](index))
	{
		++index;
	}
	return *this;
}

Entity::ComponentIterator Entity::begin() const
{
	return ++Entity::ComponentIterator(this);
}

Entity::ComponentIterator Entity::end() const
{
	return Entity::ComponentIterator(this, _components.size());
}
