#pragma once
#include <array>
#include <vector>
#include "Entity.h"
#include <ranges>
#include "common/ñontainers/sparse_array.h"


class EntityManager
{
public:
	using entity_container_type = SparseArray<Entity, EntityID, MAX_ENTITY>;
	using entity_iterator = std::vector<Entity>::iterator;
	EntityManager() : _max(-1)
	{
	}

	Entity& create() noexcept;

	bool hasEntity(EntityID id) const noexcept;
	const Entity& get(EntityID id) const noexcept;
	void registerComponent(EntityID entt, ComponentID id) noexcept;

	void unregisterComponent(EntityID entt, ComponentID id) noexcept;

	bool destroy(const Entity& entity) noexcept;


	auto begin() noexcept
	{
		return _entities.begin();
	}

	auto end() noexcept
	{
		return _entities.end();
	}

	/// @brief create view wich can iterate throught all entities
	/// @return EntityView
	auto entityView() noexcept
	{
		struct EntityView
		{
			using iterator_type = decltype(_entities.begin());
			EntityView(iterator_type b, iterator_type e) : _begin(b),_end(e){}
			auto begin() noexcept
			{
				return _begin;
			}

			auto end() noexcept
			{
				return _end;
			}
 
		private:
			const iterator_type _begin;
			const iterator_type _end;
		};
		EntityView view({ _entities.begin() ,_entities.end() });

		return view;
	}


	
private:
	entity_container_type _entities;
	std::vector<EntityID> _gaps;
	EntityID _max;
};

