#pragma once
#include <array>
#include <vector>
#include "Entity.h"
#include <ranges>
#include "../Containers/sparse_array.h"

class EntityManager
{
public:
	using entity_container_type = SparseArray<Entity, EntityID, MAX_ENTITY>;
	using entity_iterator = std::vector<Entity>::iterator;
	EntityManager() : _max(-1)
	{
		_entities.reserve(MAX_ENTITY);//TODO initialize with max entity at start
	}
	// класс итератор который будет итерировать по елементам из EntityManager но только по тем у которых будет заданыый набор компонентов передаваемый как std::array<ComponentID, N>
	template<size_t N>
	class Iterator 
	{
	public:
		Iterator(EntityManager& manager,const std::array<ComponentID,N>& ids) : _manager(manager),ids(ids) {
		
		
		}

		Iterator& operator++()
		{

			while (
				++_current <= _manager._max 
				&& !(_manager.hasEntity(_current) && _manager._entities[_current].hasComponents(ids)))
			{

			}

			return *this;
		}

		Entity& operator*()
		{
			return _manager._entities[_current];
		}

		bool operator!=(const Iterator& other) const
		{
			return _current != other._current;
		}

		bool operator==(const Iterator& other) const
		{
			return !(*this != other);
		}

		Iterator begin()
		{

			Iterator it = *this;
			it._current = -1;

			return ++it;
		}

		Iterator end()
		{
			Iterator it = *this;
			it._current = _manager._max + 1;
			return it;
		}

	private:
		EntityID _current = -1;
		EntityManager& _manager;
		const std::array<ComponentID, N> ids;
	};


	template<size_t N>
	Iterator<N> getEntitiesWith(const std::array<ComponentID, N>& ids)noexcept
	{
		return Iterator<N>(*this, ids);
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
	/// @return 
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

