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
		auto type_id = Family::Type<T>();
		_entityManager.registerComponent(entt_id, type_id);
		return _ComponentManager.addComponent<T>(entt_id);
	}

	template<>
	Transform& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		auto type_id = Family::Type<Transform>();
		_entityManager.registerComponent(entt_id, type_id);
		auto& tr = _ComponentManager.addComponent<Transform>(entt_id);
		tr.vertexConstantBuffer = VertexConstantBuffer<DirectX::XMMATRIX>(_gfx, tr.orientationMatrix);
		return tr;
	}

	template<>
	Render& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		auto type_id = Family::Type<Render>();
		_entityManager.registerComponent(entt_id, type_id);
		auto& r = _ComponentManager.addComponent<Render>(entt_id);
		r.topology= Topology(_gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return r;
	}



	template<typename T>
	void removeComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		auto type_id = Family::Type<T>();
		_entityManager.unregisterComponent(entt_id, type_id);
		_ComponentManager.removeComponent<T>(entt_id);
	}

	template<typename T>
	bool hasComponent(const Entity& entt) const noexcept
	{
		auto type_id = Family::Type<T>();
		return entt.hasComponent(type_id);
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

#include <tuple>

namespace
{
	template<class... Args>
	struct With
	{
		With(SceneManager& scene) noexcept : pools({ scene.getPool<WithArgs>()... })
		{

		}

		auto begin()
		{

		}

		auto end()
		{

		}

		std::tuple< ComponentPool<WithArgs>&...> pools;
	};

	template<class... Args>
	struct Without
	{
	};

	template<>
	struct ComponentView<>
	{
	};

	template<typename Pools,typename Entity>
	bool all_of(const Pools& pools, const Entity& entt) noexcept
	{
		constexpr auto size = std::tuple_size_v<Pools>;
		for (size_t i = 0; i < size && pools.get<i>().hasComponent(entt); ++i) {};
		return i == size;
	}
	
	template<typename Pools,typename Entity>
	bool none_of(const Pools& pools, const Entity& entt) noexcept
	{
		constexpr auto size = std::tuple_size_v<Pools>;
		for (size_t i = 0; i < size && !pools.get<i>().hasComponent(entt); ++i) {};
		return i == size;
	}

	template<class... WithArgs,class... WithoutArgs>
	struct ComponentView<With<WithArgs...>, Without<WithoutArgs...>>
	{
		ComponentView(SceneManager& scene) noexcept :
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
	struct ComponentView
	{
		using with_tuple = std::tuple< ComponentPool<WithArgs>&...>;

		ComponentView(SceneManager& scene) noexcept : _pools({ scene.getPool<WithArgs>()... })
		{}



		template<class T>
		auto& get(const Entity& entt) noexcept
		{
			static_assert(isWith<T>);
			return std::get< ComponentPool<T>&>(_pools).getComponent(entt.getID());
		}
		template<class T>
		static constexpr bool isWith = (... || std::is_same_v<T, WithArgs>);

		static constexpr size_t withN = sizeof...(WithArgs);

		class iterator
		{
		public:
			iterator(const with_tuple& view_pools ) : _pools(view_pools),it(view_pools.get<0>.begin())
			{

			}

			iterator operator++(int) const noexcept
			{
				auto copy = *this;
				++(*this);

				return copy;
			}

			iterator& operator++() noexcept
			{
				bool find = true;
				do
				{
					++it;
					const auto entt_id = *it;
					find = all_of(_pools, entt_id);
					if (all_of(_pools, entt_id))
						break;
				}while (true)

				return *this;
			}

			bool operator!=(const iterator& other) const noexcept
			{
				return it != other.it;
			}

			bool operator==(const iterator& other) const noexcept
			{
				return it == other.it;
			}

			

			with_tuple _pools;
		private:
			auto it;
			auto end;
		};

		auto begin() noexcept
		{

		}

		auto end() noexcept
		{

		}

		//this function is not check if component exist. undefined behaviour in other case
		auto get(const Entity& entt) const noexcept
		{
			auto entt_id = entt.getID();
			return std::tuple<WithArgs&...>(std::get<ComponentPool<WithArgs>&>(_pools).getComponent(entt_id)...);
		}

		with_tuple _pools;
	private:
	};

#include "../Containers/sparse_set.h"


	template<class T>
	class ComponentPool:public IComponentPool,public SparseArray<T, EntityID, MAX_ENTITY>{};
	void ftest()
	{
		ComponentPool<Transform> arr;
		
		for (auto& tr : arr)
		{

		}

		if (arr.contains(2))
		{

			auto& dat =	arr.add(2);
			arr.remove(2);
			arr.remove(2);
			dat.scale = { 1,1,1 };
		}


		for (auto it = arr.begin(); it != arr.end(); ++it)
		{

		}

		SceneManager scene;
		auto view = ComponentView < With<Transform, Render>,Without<Render>>(scene);
		auto onlyWith = ComponentView<Transform, Render>(scene);
		constexpr auto a = view.isWith<Transform>;
		auto entt = scene.createEntity();
		auto& tr = view.get<Transform>(entt);

		auto r = onlyWith.get<Render>(entt);

		all_of(onlyWith._pools, entt);

		//none_of(ids<Transform>, entt);

		auto[tr,r] = onlyWith.get(entt);

	}
}
