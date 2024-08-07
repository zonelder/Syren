#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "ComponentPool.h"
#include "FamilyGenerator.h"

class ComponentManager
{
public:
	using pool_base_type = SparseSet<EntityID, MAX_ENTITY>;

	template<unsigned N>
	struct component_traits{};
	template<typename T>
	void addPool()
	{

		constexpr ComponentID id = Family::Type<T>();
		template<>
		component_traits<id>
		{
			using component_type = T;
		};

		_pools[id] = new ComponentPool<T>();
	}

	template<typename T>
	ComponentPool<T>* getPool()
	{

		constexpr ComponentID id = Family::Type<T>();
		if (!_pools.contains(id))
			addPool<T>();

		ComponentPool<T>* ptr = static_cast<ComponentPool<T>*>(_pools[id]);
		return ptr;
	}


	template<typename T>
	T& getComponent(EntityID id)
	{
		auto type_id = Family::Type<T>();
		if (!_pools.contains(type_id))
			addPool<T>();
		ComponentPool<T>* pool = getPool<T>();
		return pool[id];
	}

	template<typename T>
	T& addComponent(EntityID id)
	{
		auto type_id = Family::Type<T>();
		if (!_pools.contains(type_id))
			addPool<T>();

		ComponentPool<T>* p_pool= getPool<T>();
		return p_pool->add(id);
	}

	template<typename T>
	bool removeComponent(EntityID id)
	{
		ComponentPool<T>* pool = getPool<T>();
		return pool->remove(id);
	}


	void removeComponent(ComponentID comp_id, EntityID entt_id)
	{
	}
	void removeAllComponents();

	~ComponentManager();

private:

	std::unordered_map<ComponentID, pool_base_type*> _pools;
};

