#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "ComponentPool.h"
#include "FamilyGenerator.h"

class ComponentManager
{
public:

	template<typename T>
	void addPool()
	{

		ComponentID id = Family::Type<T>();
		IComponentPool* cp_ptr = new ComponentPool<T>();

		_pools[id] = cp_ptr;
	}

	template<typename T>
	ComponentPool<T>* getPool()
	{

		ComponentID id = Family::Type<T>();
		if (!_pools.contains(id))
			addPool<T>();


		IComponentPool* int_ptr = _pools[id];

		ComponentPool<T>* ptr = dynamic_cast<ComponentPool<T>*>(int_ptr);
		return ptr;
	}


	template<typename T>
	T& getComponent(EntityID id)
	{
		auto type_id = Family::Type<T>();
		if (!_pools.contains(type_id))
			addPool<T>();
		ComponentPool<T>* pool = getPool<T>();
		return pool->getComponent(id);
	}

	template<typename T>
	T& addComponent(EntityID id)
	{
		auto type_id = Family::Type<T>();
		if (!_pools.contains(type_id))
			addPool<T>();

		ComponentPool<T>* p_pool= getPool<T>();

		p_pool->addComponent(id);
		return p_pool->getComponent(id);
	}

	template<typename T>
	bool removeComponent(EntityID id)
	{
		auto type_id = Family::Type<T>();
		ComponentPool<T>* pool = getPool<T>();
		bool res = pool->removeComponent(id);
		return res;
	}


	void removeComponent(ComponentID comp_id, EntityID entt_id);
	void removeAllComponents();

	~ComponentManager();

private:
	std::unordered_map<ComponentID,IComponentPool*> _pools;
};

