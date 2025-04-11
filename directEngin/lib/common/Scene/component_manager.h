#pragma once
#include "component_pool.h"
#include "family_generator.h"

#include <memory>
#include <vector>
#include <unordered_map>

class ComponentManager
{
public:
	using pool_base_type = IComponentPool;

	template<typename T>
	ComponentPool<T>* addPool()
	{
		auto type_id = Family::type_id<T>();
		ComponentPool<T>* cp_ptr = new ComponentPool<T>;
		assert(!_pools.contains(type_id));
		_pools[type_id] = cp_ptr;
		return cp_ptr;
	}

	template<typename T>
	ComponentPool<T>* getPool()
	{

		ComponentID id = Family::type_id<T>();
		if (!_pools.contains(id))
			return addPool<T>();

		ComponentPool<T>* ptr = static_cast<ComponentPool<T>*>(_pools[id]);
		return ptr;
	}

	template<typename T>
	const ComponentPool<T>* getPool() const
	{

		ComponentID id = Family::type_id<T>();
		auto it = _pools.find(id);
		if (it == _pools.end())
			return nullptr;
		const ComponentPool<T>* ptr = static_cast<const ComponentPool<T>*>(it->second);
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
		ComponentPool<T>* p_pool= getPool<T>();
		assert(p_pool != nullptr);
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

	const auto& pools() const noexcept
	{
		return _pools;
	}

private:

	std::unordered_map<ComponentID, pool_base_type*> _pools;
};

