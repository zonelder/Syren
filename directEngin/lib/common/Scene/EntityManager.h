#pragma once
#include <array>
#include <vector>
#include "Entity.h"

class EntityManager
{
public:
	Entity& create() noexcept;

	bool hasEntity(EntityID id) const noexcept;
	const Entity& get(EntityID id) const noexcept;
	void registerComponent(EntityID entt, ComponentID id) noexcept;

	void unregisterComponent(EntityID entt, ComponentID id) noexcept;

	bool destroy(const Entity& entity) noexcept;

	
private:
	std::array<Entity, MAX_ENTITY> _entities;
	EntityID _max = -1;
	std::vector<EntityID> _gaps;
};

