#pragma once
#include "Camera/Camera.h"
#include "../Window.h"
#include <unordered_map>
#include "ComponentPool.h"
#include "ComponentManager.h"
#include "../../component/Render.h"
#include "../../component/Transform.h"
#include "../Input.h"

using EntityID = unsigned int;

class SceneManager
{
public:
	SceneManager(const Window& wnd);


	Camera& getCamera() noexcept;

	Graphics& getGraphic() noexcept;


	template<typename T>
	ComponentPool<T>& getPool()
	{
		return *(_ComponentManager.getPool<T>());
	}

	template<typename T>
	T& addComponent(EntityID id)
	{
		return _ComponentManager.addComponent<T>(id);
	}

	template<>
	Transform& addComponent(EntityID id)
	{
		auto& tr = _ComponentManager.addComponent<Transform>(id);
		tr.vertexConstantBuffer = VertexConstantBuffer<DirectX::XMMATRIX>(_gfx, tr.orientationMatrix);
		return tr;
	}

	template<>
	Render& addComponent(EntityID id)
	{
		auto& r = _ComponentManager.addComponent<Render>(id);
		r.topology= Topology(_gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return r;
	}

	/// @brief create new material and return ptr to it
	/// @param vertexShader - path to compiled vertex shader
	/// @param pixelShader  - path to compiled pixel shader
	/// @return - material with a given shaders
	std::shared_ptr<Material> makeMaterial(const wchar_t* vertexShader = L"VertexShader.cso", const wchar_t* pixelShader = L"PixelShader.cso");

	/// @brief create new mesh and return ptr to it
	/// @return 
	std::shared_ptr<Mesh> makeMesh(
		const std::vector<Vertex>& vertices,
		const std::vector<unsigned short> &indices,
		const Mesh::ConstantBuffer2& colors
	);


	std::shared_ptr<Mesh> makeBoxMesh();

	std::shared_ptr<Mesh> makeCylinderMesh(unsigned int n = 8);

	std::shared_ptr<Mesh> make2SidedPlaneMesh();

	void onStartFrame();


	/// @brief update input with data from window
	/// @param wnd # window whose  as input handler
	void updateInput(const Window& wnd) noexcept;

	const Input& getInput() const noexcept;

	void onEndFrame();
private:
	Graphics _gfx;
	ComponentManager _ComponentManager;
	Camera _mainCamera;
	Input _input;
};

