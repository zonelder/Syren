#pragma once
#include "Camera/Camera.h"
#include "../Window.h"
#include <unordered_map>
#include "ComponentPool.h"
#include "ComponentManager.h"
#include "../../component/Render.h"
#include "../../component/Transform.h"
#include "../Input.h"
#include "EntityManager.h"
#include "meshPool.h"
#include <ranges>
#include <tuple>

namespace
{
	template<class ...Args>
	std::array<ComponentID, sizeof...(Args)> ids = { Family::type_id<Args>()...};

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

namespace
{
	template<class... Args>
	class ComponentView;

	template<class... Args>
	struct With
	{
	};

	template<class... Args>
	struct Without
	{
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

	// почему класс итератора может быть не доступен вне класса EntityManager
	template<typename ... Args>
	EntityManager::Iterator<sizeof...(Args)>  getEntitiesWith() noexcept
	{
		std::array<ComponentID, sizeof...(Args)> ids = { Family::type_id<Args>()...};

		return _entityManager.getEntitiesWith(ids);
	}
	


	template<class... Args> requires (sizeof...(Args) > 0)
	auto& view() noexcept
	{
		using with = With<Args...>;
		using without = Without<>;
		static ComponentView<with, without> m(_ComponentManager);// create view on each filter once and then reuse it
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


	template<typename T>
	T& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		_entityManager.registerComponent(entt_id, Family::type_id<T>());
		return _ComponentManager.addComponent<T>(entt_id);
	}

	template<>
	Transform& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		_entityManager.registerComponent(entt_id, Family::type_id<Transform>());
		auto& tr = _ComponentManager.addComponent<Transform>(entt_id);
		tr.vertexConstantBuffer = VertexConstantBuffer<DirectX::XMMATRIX>(_gfx, tr.orientationMatrix);
		return tr;
	}

	template<>
	Render& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		_entityManager.registerComponent(entt_id, Family::type_id<Render>());
		auto& r = _ComponentManager.addComponent<Render>(entt_id);
		r.topology= Topology(_gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return r;
	}



	template<typename T>
	void removeComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		_entityManager.unregisterComponent(entt_id, Family::type_id<T>());
		_ComponentManager.removeComponent<T>(entt_id);
	}

	template<typename T>
	bool hasComponent(const Entity& entt) const noexcept
	{
		return entt.hasComponent(Family::type_id<T>());
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


	template<>
	class ComponentView<>
	{
	};

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
			iterator(with_tuple& include,without_tuple exclude, entity_iterator_type it, entity_iterator_type end) noexcept : _include(include), _exclude(exclude), _it(it), _end(end)
			{

			}

			iterator(with_tuple& include, without_tuple exclude, entity_iterator_type it) noexcept : iterator(include, exclude, it, it) {}

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
				return std::make_tuple<const EntityID&, WithArgs&...>(entt, (std::get<ComponentPool<WithArgs>*>(_include)->operator[](entt))...);
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
		static constexpr bool isWith =  (... || std::is_same_v<T, WithArgs>);

		iterator begin() noexcept
		{
			auto& pool = std::get<0>(_includes);
			return iterator(_includes, _excludes, pool->ebegin(), pool->eend());
		}

		iterator end() noexcept
		{
			auto& pool = std::get<0>(_includes);
			return iterator(_includes, _excludes, pool->eend());
		}

	private:
		with_tuple _includes;
		without_tuple _excludes;
	};
	/*
	template<class... WithArgs>
	class ComponentView
	{
	public:
		using with_tuple = std::tuple< ComponentPool<WithArgs>*...>;
		using entity_iterator = std::vector<EntityID>::iterator;

		ComponentView(with_tuple&& pools) noexcept : _pools(pools) {}

		ComponentView(ComponentManager& scene) noexcept : _pools({ scene.getPool<WithArgs>()... })
		{}



		template<class T>
		auto& get(const Entity& entt) noexcept
		{
			return get<T>(entt.getID());
		}

		template<class T>
		auto& get(const EntityID& entt) noexcept
		{
			static_assert(isWith<T>);
			return std::get<ComponentPool<T>*>(_pools)->operator[](entt);
		}

		template<class T>
		static constexpr bool isWith = (... || std::is_same_v<T, WithArgs>);

		static constexpr size_t withN = sizeof...(WithArgs);

		class iterator
		{
			using entity_iterator_type = entity_iterator;
		public:
			iterator(with_tuple& view_pools, entity_iterator_type it, entity_iterator_type end) noexcept : _pools(view_pools),_it(it),_end(end)
			{

			}

			iterator(with_tuple& view_pools, entity_iterator_type it) noexcept : iterator(view_pools, it, it) {}

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
					if (all_of(_pools, *_it))
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
				return std::make_tuple<const EntityID&, WithArgs&...>(entt, (std::get<ComponentPool<WithArgs>*>(_pools)->operator[](entt))...);
			}

			
		private:

			with_tuple& _pools;
			entity_iterator_type _it;
			entity_iterator_type _end;
		};

		iterator begin() noexcept
		{
			auto& pool = std::get<0>(_pools);
			return iterator(_pools, pool->ebegin(), pool->eend());
		}

		iterator end() noexcept
		{
			auto& pool = std::get<0>(_pools);
			return iterator(_pools, pool->eend());
		}

		/*
		//this function is not check if component exist. undefined behaviour in other case
		auto get(const Entity& entt) const noexcept
		{
			auto entt_id = entt.getID();
			return std::tuple<WithArgs&...>(std::get<typle_element_type>(_pools)[entt_id]...);
		}
		* /
	private:
		with_tuple _pools;
	};
	*/

	void ftest()
	{
		SparseArray<Transform, EntityID, MAX_ENTITY> pool;
		SparseSet<EntityID, MAX_ENTITY> constexprTest;

		std::tuple<ComponentPool<Transform>*, ComponentPool<Render>*> t;
		//auto b = all_of(t, 1);
	}
		
	template<typename... Args>
	void f1(){}

	template<size_t ...Args>
	struct st
	{

	};

	template<size_t ...Args>
	void f2()
	{
		//st<Args...> a;
		f1();
	}


	void f3()
	{
		f2<2, 2>();
	}
}