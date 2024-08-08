#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "ComponentPool.h"
#include "FamilyGenerator.h"

class ComponentManager
{
public:
	using pool_base_type = IComponentPool;

	template<typename T>
	void addPool()
	{
		pool_base_type* cp_ptr = new ComponentPool<T>;

		_pools[Family::Type<T>()] = cp_ptr;
	}

	template<typename T>
	ComponentPool<T>* getPool()
	{

		ComponentID id = Family::Type<T>();
		if (!_pools.contains(id))
			addPool<T>();

		ComponentPool<T>* ptr = dynamic_cast<ComponentPool<T>*>(_pools[id]);
		return ptr;
	}


	template<typename T>
	T& getComponent(EntityID id)
	{
		ComponentPool<T>* pool = getPool<T>();
		return pool->operator[](id);
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

	bool removeComponent(ComponentID compID, EntityID id)
	{
		return  _pools[compID]->removeComponent(id);
	}

	~ComponentManager();

private:

	std::unordered_map<ComponentID, pool_base_type*> _pools;
};

