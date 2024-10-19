#pragma once
#include "icomponent_pool.h"
#include <unordered_map>

#include "common/ñontainers/sparse_array.h"

template<typename T>
class ComponentPool final :public IComponentPool, public SparseArray<T, EntityID, MAX_ENTITY>
{
	using base_pool_type = SparseArray<T, EntityID, MAX_ENTITY>;
public:
	ComponentPool() = default;

	auto& getComponent(EntityID entt)
	{
		return this->operator[](entt);
	}

	bool hasComponent(EntityID entt)
	{
		return this->contains(entt);
	}

	bool removeComponent(EntityID id) override
	{
		this->remove(id);
		return true;
	}

};
