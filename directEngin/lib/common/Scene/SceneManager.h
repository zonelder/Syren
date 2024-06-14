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
	EntityManager::Iterator<sizeof...(Args)>  getEntitiesWith()
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
		auto type_id = Family::Type<Render>();
		_entityManager.unregisterComponent(entt_id, type_id);
		_ComponentManager.removeComponent<T>(entt_id);
	}

	template<typename T>
	bool hasComponent(const Entity& entt)
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

