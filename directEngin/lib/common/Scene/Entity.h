#pragma once
#include <bitset>
#include "SceneBase.h"

using ComponentSet = std::bitset<MAX_COMPONENT>;

class Entity
{
public:

	/// @brief iterator through all existing components of entity
	class ComponentIterator
	{
	public:
		ComponentIterator(const Entity*);
		ComponentIterator(const Entity*, ComponentID index);

		bool operator!=(const ComponentIterator& other) const;
		bool operator==(const ComponentIterator& other) const;
		ComponentID operator*() const;
		ComponentIterator& operator++();

	private:
		const ComponentSet* _comps;
		ComponentID index;
	};

	Entity();
	explicit Entity(EntityID id);

	Entity& operator=(const Entity& other);

	void registerComponent(ComponentID component_id) noexcept;

	void unregisterComponent(ComponentID component_id) noexcept;

	bool hasComponent(ComponentID id) const noexcept;

	/// @brief return iterator pointing on id of first existing component of entity
	/// @return 
	ComponentIterator begin() const;

	ComponentIterator end() const;

	const EntityID getID() const noexcept;
private:
	EntityID _ID;
	ComponentSet _components;
};

