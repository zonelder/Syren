#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "ComponentPool.h"
#include "IComponentPool.h"
#include "FamilyGenerator.h"
#include <any>
#include <typeinfo>


using EntityID = unsigned int;

class ComponentManager
{
public:

	template<typename T>
	void addPool()
	{

		ComponentID id = Family::Type<T>();

		_pools[id].emplace<ComponentPool<T>>();
	}

	template<typename T>
	ComponentPool<T>& getPool()
	{

		ComponentID id = Family::Type<T>();
		return std::any_cast<ComponentPool<T>&>(_pools[id]);
	}


	template<typename T>
	T& getComponent(EntityID id)
	{
		auto type_id = Family::Type<T>();
		if (!_pools.contains(type_id))
			addPool<T>();
		ComponentPool<T>& pool = getPool<T>();
		return pool.getComponent(id);
	}

	template<typename T>
	T& addComponent(EntityID id)
	{
		auto type_id = Family::Type<T>();
		if (!_pools.contains(type_id))
			addPool<T>();
		return getPool<T>().addComponent(id);
	}

	void removeAllComponents();

	~ComponentManager();


private:
	std::unordered_map<ComponentID,std::any> _pools;
};

