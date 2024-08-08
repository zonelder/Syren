#pragma once
#include <array>
#include <vector>
#include "Entity.h"
#include <ranges>


class EntityManager
{
public:
	EntityManager()
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

	constexpr auto entities() const noexcept
	{
		return _entities | std::views::filter([](const Entity& entt)->bool {
			return entt.isValid();
			});
	}

	Entity& create() noexcept;

	bool hasEntity(EntityID id) const noexcept;
	const Entity& get(EntityID id) const noexcept;
	void registerComponent(EntityID entt, ComponentID id) noexcept;

	void unregisterComponent(EntityID entt, ComponentID id) noexcept;

	bool destroy(const Entity& entity) noexcept;

	
private:
	std::vector<Entity> _entities;
	EntityID _max = -1;
	std::vector<EntityID> _gaps;
};

