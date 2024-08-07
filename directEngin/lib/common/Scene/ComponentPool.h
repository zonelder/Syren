#pragma once
#include "IComponentPool.h"
#include <unordered_map>


template<typename T>
class ComponentPool:public IComponentPool
{
public:
	
	using iterator = std::unordered_map<EntityID,T>::iterator;

	/// @brief Create new component and add it to entity
	/// @return created component
	void addComponent(EntityID id) override
	{
		auto& comp = _components[id];// TODO BETTER INIT
	}


	/// @brief Get link to entity's component. if component does not exist then create a new one
	T& getComponent(EntityID id)
	{
		return _components.at(id);
	}

	/// @brief Check is there a component of entity 
	bool hasComponent(EntityID id) const override
	{
		return _components.contains(id);
	}

	/// @brief Remove Component from the pool. if there are not any component of entity - do nothing 
	/// @param  
	bool removeComponent(EntityID id) override
	{

		if (!_components.contains(id))
			return false;
		_components.erase(id);
		return true;
	}

	void removeAll() override
	{
		_components.clear();
	}

	iterator begin() noexcept
	{
		return _components.begin();
	}

	iterator end() noexcept
	{
		return _components.end();
	}


private:

	std::unordered_map<EntityID, T> _components;
};

