#pragma once
#include "Camera/Camera.h"
#include "../window.h"
#include <unordered_map>
#include "ComponentPool.h"
#include "ComponentManager.h"
#include "components/render.h"
#include "components/transform.h"
#include "../input.h"
#include "EntityManager.h"
#include "meshPool.h"
#include <ranges>
#include <tuple>

namespace to_future
{
	namespace constexpr_filters
	{
		template<class ...Args>
		std::array<ComponentID, sizeof...(Args)> ids = { Family::type_id<Args>()... };

		template<class ...Args>
		inline consteval auto with() noexcept
		{
			return std::views::filter([](const Entity& entt)->bool {
				return entt.hasComponents(ids<Args...>);
				});
		}

		template<class ...Args>
		inline constexpr auto without() noexcept
		{
			return std::views::filter([](const Entity& entt)->bool {
				return entt.hasNotComponents(ids<Args...>);
				});
		}
	}

}

namespace
{


	template<class Wt,class Wtout>
	class ComponentView;
	struct filter_type{};
	template<class... Args>
	struct With
	{
		using ComponentFilterWith = filter_type;
	};

	template<class... Args>
	struct Without
	{
		using ComponentFilterWithout = filter_type;
	};

	template<class TWith, class TWithout>
	concept is_component_filters = requires(TWith tw, TWithout twout)
	{
		typename TWith::ComponentFilterWith;
		typename TWithout::ComponentFilterWithout;
	};
}

class SceneManager : private MeshPool
{
public:
	SceneManager(const Window& wnd);


	Camera& getCamera() noexcept;

	Graphics& getGraphic() noexcept;

	const Entity& createEntity() noexcept;

	bool destroyEntity(const Entity& entt) noexcept;

	bool destroyEntity(EntityID id) noexcept;

	

	/// @brief create simple view with include types only
	/// @tparam ...Args List of types we want to include in filter
	/// @return ComponentView
	template<class... Args>
	auto& view() noexcept
	{
		using with = With<Args...>;
		using without = Without<>;
		return view <with, without>();
	}
	/*
	/// @brief create component view with includes\excludes
	/// @tparam ...WithArgs  List of types we want to include in filter
	/// @tparam ...WithoutArgs  List of types we want to exclude from filter
	/// @return ComponentView
	template<class... WithArgs,class... WithoutArgs>
	auto& view<With<WithArgs...>,Without<WithoutArgs...>>() noexcept
	{
		using with = With<WithArgs...>;
		using without = Without<WithoutArgs...>;
		static ComponentView<with, without> m(_ComponentManager);// create view on each filter once and then reuse it
		return m;
	}
	*/

	/// @brief create view by type of other view.
	/// @tparam TWith
	/// @tparam TWithout
	/// @return ComponentView
	template<class TWith, class TWithout = Without<>> requires is_component_filters< TWith, TWithout>
	auto& view() noexcept
	{
		// TODO each filter can be crate once if component manageer wont relocate em view on each filter once and then reuse it
		static ComponentView<TWith, TWithout> m(_ComponentManager);
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

	// реализация метода getComponent
	template<typename T>
	T& getComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		return _ComponentManager.getComponent<T>(entt_id);
	}
	// реализация метода getComponent
	template<typename T>
	T& getComponent(const EntityID& entt)
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
	T& addComponent(const EntityID& entt)
	{
		_entityManager.registerComponent(entt, Family::type_id<T>());
		return _ComponentManager.addComponent<T>(entt);
	}

	template<>
	Transform& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		auto& tr = addComponent<Transform>(entt_id);
		tr.vertexConstantBuffer = VertexConstantBuffer<DirectX::XMMATRIX>(_gfx, tr.orientationMatrix);
		return tr;
	}

	template<>
	Render& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		auto& r = addComponent<Render>(entt_id);
		r.topology= Topology(_gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return r;
	}



	template<typename T>
	void removeComponent(const Entity& entt)
	{
		removeComponent<T>(entt.getID());
	}

	template<typename T>
	void removeComponent(const EntityID& entt)
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
	bool hasComponent(const EntityID& entt) const noexcept
	{
		return _entityManager.get(entt).hasComponent(Family::type_id<T>());
	}

	/// @brief create new material and return ptr to it
	/// @param vertexShader - path to compiled vertex shader
	/// @param pixelShader  - path to compiled pixel shader
	/// @return - material with a given shaders
	std::shared_ptr<Material> makeMaterial(const wchar_t* vertexShader = L"VertexShader.cso", const wchar_t* pixelShader = L"PixelShader.cso");

	/// @brief create new mesh and return ptr to it
	/// @return 
	Mesh* makeMesh(
		const std::vector<Vertex>& vertices,
		const std::vector<unsigned short> &indices,
		const MeshIternal::ConstantBuffer2& colors
	);


	MeshIternal* getMeshData(Mesh* meshComponent) const noexcept;


	Mesh* makeBoxMesh();

	Mesh* makeCylinderMesh(unsigned int n = 8);

	Mesh* make2SidedPlaneMesh();

	void onStartFrame();


	/// @brief update input with data from window
	/// @param wnd # window whose  as input handler
	void updateInput(const Window& wnd) noexcept;

	const Input& getInput() const noexcept;

	void onEndFrame();

	const Entity& getEntity(EntityID id) const noexcept;
private:
	Graphics _gfx;
	ComponentManager _ComponentManager;
	EntityManager _entityManager;
	Camera _mainCamera;
	Input _input;
};
namespace
{

	template<typename Pools,typename Entity,size_t I = 0>
	inline bool all_of(const Pools& pools, const Entity& entt) noexcept
	{
		if constexpr(I >= std::tuple_size_v<Pools>)
			return true;
		else
		{
			if (!std::get<I>(pools)->contains(entt))
				return false;
			return all_of<Pools, Entity, I + 1>(pools, entt);
		}
	}
	
	template<typename Pools,typename Entity,size_t I = 0>
	inline bool none_of(const Pools& pools, const Entity& entt) noexcept
	{
		if constexpr (I >= std::tuple_size_v<Pools>)
			return true;
		else
		{
			if (std::get<I>(pools)->contains(entt))
				return false;
			return none_of<Pools, Entity, I + 1>(pools, entt);
		}
	}

	template<class... WithArgs,class... WithoutArgs>
	class ComponentView<With<WithArgs...>, Without<WithoutArgs...>>
	{
	public:

		using with_tuple = std::tuple< ComponentPool<WithArgs>*...>;
		using without_tuple = std::tuple<ComponentPool< WithoutArgs>*...>;
		using entity_iterator = std::vector<EntityID>::iterator;

		ComponentView(ComponentManager& scene) noexcept :
			_includes({ scene.getPool<WithArgs>()... }),
			_excludes({ scene.getPool<WithoutArgs>()... })
		{

		}

		class iterator
		{
			using entity_iterator_type = entity_iterator;
		public:
			iterator(with_tuple& include,without_tuple& exclude, entity_iterator_type it, entity_iterator_type end) noexcept : _include(include), _exclude(exclude), _it(it), _end(end)
			{

			}

			iterator(with_tuple& include, without_tuple& exclude, entity_iterator_type it) noexcept : iterator(include, exclude, it, it) {}

			iterator operator++(int) const noexcept
			{
				auto copy = *this;
				++(*this);

				return copy;
			}

			iterator& operator++() noexcept
			{
				while (++_it != _end)
				{
					auto entt = *_it;
					if (all_of(_include, entt) && none_of(_exclude, entt))
						break;
				}
				return *this;
			}

			bool operator!=(const iterator& other) const noexcept
			{
				return _it != other._it;
			}

			bool operator==(const iterator& other) const noexcept
			{
				return _it == other._it;
			}

			auto operator*() noexcept
			{
				const auto& entt = *_it;
				return std::tuple<const EntityID&, WithArgs&...>(
					entt,
					(std::forward<WithArgs&>(
						std::get<ComponentPool<WithArgs>*>(_include)->operator[](entt)
					)
					)...);
			}


		private:

			with_tuple& _include;
			without_tuple& _exclude;
			entity_iterator_type _it;
			entity_iterator_type _end;
		};


		template<class T>
		auto& get(const Entity& entt) noexcept
		{
			static_assert(isWith<T>);
			return std::get< ComponentPool<T>*>(_includes)->operator[](entt.getID());
		}

		template<class T>
		auto& get(const EntityID& entt) noexcept
		{
			static_assert (isWith<T>);
			return std::get< ComponentPool<T>*>(_includes)->operator[](entt);
		}


		template<class T>
		auto contains(const EntityID& entt) noexcept
		{
			return all_of(_includes, entt) && none_of(_excludes, entt);
		}

		template<class T>
		auto has(const EntityID& entt) noexcept
		{
			static_assert (isWith<T>);
			return std::get< ComponentPool<T>*>(_includes)->contains(entt);
		}

		template<class T>
		static constexpr bool isWith =  (... || std::is_same_v<T, WithArgs>);

		iterator begin() noexcept
		{
			auto* pool = std::get<0>(_includes);
			return iterator(_includes, _excludes, pool->ebegin(), pool->eend());
		}

		iterator end() noexcept
		{
			auto* pool = std::get<0>(_includes);
			return iterator(_includes, _excludes, pool->eend());
		}

	private:
		with_tuple _includes;
		without_tuple _excludes;
	};
}