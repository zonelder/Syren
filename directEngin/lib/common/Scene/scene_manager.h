#pragma once
#include <unordered_map>
#include <functional>

#include "common/window.h"
#include "common/input.h"

#include "Camera/camera.h"

#include "resmngr/resource_manager.h"
#include "component_pool.h"
#include "component_manager.h"

#include "components/render.h"
#include "components/transform.h"

#include "entity_manager.h"
#include "filters.h"



/// @brief Some usefull context of app.its support overriding of context but  its always should be at least one instance of class.
/// in commpon case class App provide such an instance so be carefull when you overriding mainContext;
class SceneContext
{
public:
	/// @brief create new instance of scene context.its forbidden to pass nullptr to this constructor.
	///cause contest SHALL BE SERTAN AND WELL DEFINED.
	SceneContext(ResourceManager* pRes, Graphics* pGfx);
	~SceneContext();


	/// @brief get main resource manager of application
	/// @return 
	static ResourceManager* pResources() noexcept { return s_pMainContext->_pResourceManager; }

	/// @brief get main graphic handler of application
	/// @return 
	static Graphics* pGfx() noexcept { return s_pMainContext->_pGraphics; }

	static void setMainContext(SceneContext* context) noexcept 
	{
		assert(context != nullptr);
		s_pMainContext = context;
	}

private:
	ResourceManager* _pResourceManager;
	Graphics* _pGraphics;

	static std::vector<SceneContext*> s_contexts;
	static SceneContext* s_pMainContext;

};


class SceneManager
{
public:
	SceneManager(const Window& wnd);


	Camera& getCamera() noexcept;

	const Entity& createEntity() noexcept;

	bool destroyEntity(const Entity& entt) noexcept;

	bool destroyEntity(EntityID id) noexcept;

	
	/// @brief create simple view with include types only
	/// @tparam ...Args List of types we want to include in filter
	/// @return ComponentView
	template<class... Args> requires filters::has_not_filters<Args...>
	auto& view() noexcept
	{
		using with = filters::With<Args...>;
		using without = filters::Without<>;

		constexpr auto b = filters::is_component_filters<with, without>;
		return view<with, without>();
	}

	/// @brief create view by type of other view.
	/// @tparam TWith
	/// @tparam TWithout
	/// @return ComponentView
	template<class TWith, class TWithout = filters::Without<>> requires filters::is_component_filters< TWith, TWithout>
	auto& view() noexcept
	{
		// TODO each filter can be crate once if component manageer wont relocate em view on each filter once and then reuse it
		static filters::ComponentView<TWith, TWithout> m(_ComponentManager);
		return m;
	}
	

	/// @brief return view of all entities
	/// @return 
	auto view() noexcept
	{
		return _entityManager.entityView();
	}

	template<typename T>
	ComponentPool<T>& getPool()
	{
		return *(_ComponentManager.getPool<T>());
	}
	template<typename T>
	const ComponentPool<T>& getPool() const
	{
		return *(_ComponentManager.getPool<T>());
	}

	template<typename T>
	ComponentPool<T>& getPoolByGuid(std::string& guid)
	{

	}
	// ���������� ������ getComponent
	template<typename T>
	T& getComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		return _ComponentManager.getComponent<T>(entt_id);
	}
	// ���������� ������ getComponent
	template<typename T>
	T& getComponent(EntityID entt)
	{
		return _ComponentManager.getComponent<T>(entt);
	}


	template<typename T>
	T& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		return addComponent<T>(entt_id);
	}
	template<typename T>
	T& addComponent(EntityID entt)
	{
		_entityManager.registerComponent(entt, Family::type_id<T>());
		return _ComponentManager.addComponent<T>(entt);
	}

	template<>
	Render& addComponent(EntityID entt)
	{
		_entityManager.registerComponent(entt, Family::type_id<Render>());
		auto& r = _ComponentManager.addComponent<Render>(entt);
		r.topology= Topology(*SceneContext::pGfx(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return r;
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

	MeshPtr make2SidedPlaneMesh();

	void onStartFrame();


	/// @brief update input with data from window
	/// @param wnd # window whose  as input handler
	void updateInput(const Window& wnd) noexcept;

	const Input& getInput() const noexcept;

	void onEndFrame();

	const Entity& getEntity(EntityID id) const noexcept;

	const auto& pools() const noexcept
	{
		return _ComponentManager.pools();
	}

	std::unordered_map<int, EntityID>  instantiate(FbxPrefabPtr pPrefab);

private:
	ComponentManager _ComponentManager;
	EntityManager _entityManager;
	Camera _mainCamera;
	Input _input;
};