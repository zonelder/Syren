#pragma once

#include "camera.h"
#include "component_pool.h"
#include "component_manager.h"
#include "entity_manager.h"
#include "filters.h"

#include "core/window.h"
#include "core/input.h"


/**
 * @class SceneManager
 * @brief Central management class for scene entities, components, and systems
 * 
 * Handles entity creation/destruction, component management, and provides views
 * for iterating through component combinations.
 */
class SceneManager
{
public:

    /**
     * @brief Get the main camera reference
     * @return Reference to the scene's main camera
     */
	Camera& getCamera() noexcept;

    /**
     * @brief Create a new entity
     * @return Reference to the newly created entity
     */
	const Entity& createEntity() noexcept;

    /**
     * @brief Destroy an entity by reference
     * @param entt Entity to destroy
     * @return true if destruction succeeded, false otherwise
     */
	bool destroy(const Entity& entt) noexcept;

    /**
     * @brief Destroy an entity by ID
     * @param id ID of entity to destroy
     * @return true if destruction succeeded, false otherwise
     */
	bool destroy(EntityID id) noexcept;

	
    /**
     * @brief Create a component view with specified included components
     * @tparam Args List of component types to include in the view
     * @return ComponentView configured with specified filters
     */
	template<class... Args> requires (sizeof...(Args) > 0) && filters::is_filter_free<Args...>
	auto& view() noexcept
	{
		using with = filters::With<Args...>;
		using without = filters::Without<>;
		return view<with, without>();
	}

    /**
     * @brief Create a component view with inclusion/exclusion filters
     * @tparam WithArgs Component types to include in the view
     * @tparam WithoutArgs Component types to exclude from the view
     * @return ComponentView configured with specified filters
     * 
     * @note The view is cached per filter combination type for efficiency
     * @example view<Transform, Render>() returns view of entities with both components
     */
	template<typename With, typename Without = filters::Without<>>
		requires filters::is_with_filter_v<With> && filters::is_without_filter_v<Without>
	auto& view() noexcept
	{
		// TODO each filter can be crate once if component manageer wont relocate em view on each filter once and then reuse it
		static filters::ComponentView<With,Without> m(_ComponentManager);
		return m;
	}
	
    /**
     * @brief Get a view of all entities in the scene
     * @return EntityView providing access to all entities
     */
	auto view() noexcept
	{
		return _entityManager.entityView();
	}

    /**
     * @brief Get component pool (deprecated)
     * @tparam T Type of component to get pool for
     * @return Reference to component pool
     * @deprecated Use view<T>() instead
     */
    template<typename T>
    //[[deprecated("Use view<T>() instead")]] 
    ComponentPool<T>& getPool()
    {
        return *(_ComponentManager.getPool<T>());
    }

    /**
     * @brief Get component pool (deprecated)
     * @tparam T Type of component to get pool for
     * @return Reference to component pool
     * @deprecated Use view<T>() instead
     */
	template<typename T>
	//[[deprecated("Use view<T>() instead")]]
	const ComponentPool<T>& getPool() const
	{
		return *(_ComponentManager.getPool<T>());
	}

	/**
	 * @brief Retrieve a component from an entity
	 * @tparam T Type of component to retrieve
	 * @param entt Entity reference to get component from
	 * @return Reference to the component
	 */
	template<typename T>
	T& getComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		return _ComponentManager.getComponent<T>(entt_id);
	}
	/**
	 * @brief Retrieve a component from an entity
	 * @tparam T Type of component to retrieve
	 * @param entt Entity reference to get component from
	 * @return Reference to the component
	 * @throws std::runtime_error if component doesn't exist
	 */
	template<typename T>
	T& getComponent(EntityID entt)
	{
		return _ComponentManager.getComponent<T>(entt);
	}

	/**
	 * @brief Add component to entity
	 * @tparam T Type of component to add
	 * @param entt Entity to add component to
	 * @return Reference to newly created component
	 * @post Entity's component mask is updated
	 */
	template<typename T>
	T& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		return addComponent<T>(entt_id);
	}
	/**
	 * @brief Add component to entity
	 * @tparam T Type of component to add
	 * @param entt Entity to add component to
	 * @return Reference to newly created component
	 * @post Entity's component mask is updated
	 */
	template<typename T>
	T& addComponent(EntityID entt)
	{
		_entityManager.registerComponent(entt, Family::type_id<T>());
		return _ComponentManager.addComponent<T>(entt);
	}

	template<typename T>
	void removeComponent(const Entity& entt)
	{
		removeComponent<T>(entt.getID());
	}

	template<typename T>
	void removeComponent(EntityID entt)
	{
		_entityManager.unregisterComponent(entt, Family::type_id<T>());
		_ComponentManager.removeComponent<T>(entt);
	}

	template<typename T>
	bool hasComponent(const Entity& entt) const noexcept
	{
		return entt.hasComponent(Family::type_id<T>());
	}
	template<typename T>
	bool hasComponent(EntityID entt) const noexcept
	{
		return _entityManager.get(entt).hasComponent(Family::type_id<T>());
	}
;
	/**
	 * @brief Get entity by ID
	 * @param id Entity ID to look up
	 * @return const Reference to the entity
	 */
	const Entity& getEntity(EntityID id) const noexcept;

	const auto& pools() const noexcept
	{
		return _ComponentManager.pools();
	}

private:
	ComponentManager _ComponentManager;
	EntityManager _entityManager;
	Camera _mainCamera;
};