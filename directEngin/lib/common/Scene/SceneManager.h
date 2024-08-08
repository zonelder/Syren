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
	constexpr std::array<ComponentID, sizeof...(Args)> ids = { Family::Type<Args>()... };

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
		std::array<ComponentID, sizeof...(Args)> ids = { Family::Type<Args>()... };

		return _entityManager.getEntitiesWith(ids);
	}


	constexpr auto entities() const noexcept
	{
		return _entityManager.entities();
	}
	
	/*
	template<class... Args>
	auto& view() noexcept
	{
		static ComponentView< Args...> m(_ComponentManager);// create view on each filter once and then reuse it
		return m;
	}
	*/


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
		_entityManager.registerComponent(entt_id, Family::Type<T>());
		return _ComponentManager.addComponent<T>(entt_id);
	}

	template<>
	Transform& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		_entityManager.registerComponent(entt_id, Family::Type<Transform>());
		auto& tr = _ComponentManager.addComponent<Transform>(entt_id);
		tr.vertexConstantBuffer = VertexConstantBuffer<DirectX::XMMATRIX>(_gfx, tr.orientationMatrix);
		return tr;
	}

	template<>
	Render& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		_entityManager.registerComponent(entt_id, Family::Type<Render>());
		auto& r = _ComponentManager.addComponent<Render>(entt_id);
		r.topology= Topology(_gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return r;
	}



	template<typename T>
	void removeComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		_entityManager.unregisterComponent(entt_id, Family::Type<T>());
		_ComponentManager.removeComponent<T>(entt_id);
	}

	template<typename T>
	bool hasComponent(const Entity& entt) const noexcept
	{
		return entt.hasComponent(Family::Type<T>());
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
/*
#include <tuple>

namespace
{
	template<class... Args>
	struct With
	{
	};

	template<class... Args>
	struct Without
	{
	};

	template<>
	struct ComponentView<>
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
		if (std::get<I>(pools)->contains(entt))
			return false;
		return none_of<Pools, Entity, I + 1>(pools, entt);
	}

	template<class... WithArgs,class... WithoutArgs>
	class ComponentView<With<WithArgs...>, Without<WithoutArgs...>>
	{
	public:
		ComponentView(ComponentManager& scene) noexcept :
			_includes({ scene.getPool<WithArgs>()... }),
			_excludes({ scene.getPool<WithoutArgs>()... })
		{

		}

		class iterator
		{

		};


		template<class T>
		auto& get(const Entity& entt) noexcept
		{
			static_assert(isWith<T>);
			return std::get< ComponentPool<T>&>(_includes).getComponent(entt.getID());
		}

		template<class T>
		static constexpr bool isWith =  (... || std::is_same_v<T, WithArgs>);

		auto begin() noexcept
		{
			
		}

		auto end() noexcept
		{

		}

	private:
		std::tuple< ComponentPool<WithArgs>&...> _includes;
		std::tuple<ComponentPool< WithoutArgs>&...> _excludes;
	};


	template<class... WithArgs>
	class ComponentView
	{
	public:
		using with_tuple = std::tuple< ComponentPool<WithArgs>*...>;
		using entity_iterator = std::vector<EntityID>::iterator;

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
			return std::get<ComponentPool<T>*>(_pools)[entt];
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

#include "../Containers/sparse_array.h"


	void ftest()
	{
		SparseArray<Transform, EntityID, MAX_ENTITY> pool;
		SparseSet<EntityID, MAX_ENTITY> constexprTest;

		std::tuple<ComponentPool<Transform>*, ComponentPool<Render>*> t;
		//auto b = all_of(t, 1);
	}
}
*/