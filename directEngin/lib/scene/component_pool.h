#ifndef __COMPONENT_POOL_H__
#define __COMPONENT_POOL_H__

#include "icomponent_pool.h"
#include "common/ñontainers/sparse_array.h"
#include "common/ñontainers/sparse_page.h"

/**
 * @brief Base Component Pool. Store component of exact type and provide acces to its modification
 * @tparam T - # component type
 */
template<typename T>
class ComponentPool final :public IComponentPool, public SparsePage<T,EntityID>
{
public:
	ComponentPool() = default;

	/**
	 * \param entt - entity id
	 * \return reference to entity component of type T
	 */
	T& getComponent(EntityID entt)
	{
		return this->get(entt);
	}
	/**
	 * Check if component exist in current entity.
	 * 
	 * \param entt entity id
	 * \return true if component exist, false otherwise.
	 */
	bool hasComponent(EntityID entt)
	{
		return this->contains(entt);
	}
	/**
	 * remove component from entity if it exist..
	 * 
	 * \param id entity id
	 * \return true if component was removed,false otherwise
	 */
	bool removeComponent(EntityID id) override
	{
		this->remove(id);
		return true;
	}
};

#endif