#include "scene_manager.h"
#include "components/parent.h"



const Entity& SceneManager::createEntity() noexcept
{
	return _entityManager.create();
}

bool SceneManager::destroyEntity(const Entity& entt) noexcept
{
	return destroyEntity(entt.getID());
}

bool SceneManager::destroyEntity(EntityID entt_id) noexcept
{

	bool res = _entityManager.hasEntity(entt_id);
	if (!res) return false;
	const Entity& entt = _entityManager.get(entt_id);


	for (auto it = entt.begin(); it != entt.end(); ++it)
	{

		_ComponentManager.removeComponent(*it, entt_id);

	}

	_entityManager.destroy(entt);
	std::cout << "[Scene]Destroy entity " << entt_id << std::endl;

	return true;
}


void SceneManager::onStartFrame()
{
	_mainCamera.OnFrame();
}

void SceneManager::onEndFrame()
{}

const Entity& SceneManager::getEntity(EntityID id) const noexcept
{
	return _entityManager.get(id);
}

std::unordered_map<int, EntityID> SceneManager::instantiate(FbxPrefabPtr pPrefab)
{
	if (!pPrefab)
		return {};

	std::unordered_map<int, EntityID> indexToID;
	int index = 0;
	for (auto& node : pPrefab->getNodes())
	{
		const Entity& entt = createEntity();
		indexToID[index] = entt.getID();
		++index;
		auto& transform = addComponent<Transform>(entt);
		transform.position = node.position;
		transform.rotation = DirectX::XMVectorSet(node.rotation.x, node.rotation.y, node.rotation.z, node.rotation.w);
		//transform.scale = node.scale;

		if (node.meshID != -1)
		{
			auto& render = addComponent<Render>(entt);
			render.pMesh = pPrefab->getMesh(node.meshID);
			render.pMaterial = context::getMaterial("resource/example/tile_test/material/tile_black.syrenmaterial");//some default material
		}

		if (node.parent != -1)
		{
			auto& parent = addComponent<Parent>(entt);
			parent.parent = indexToID[node.parent];
		}
	}
	return indexToID;
}

Camera& SceneManager::getCamera() noexcept
{
	return _mainCamera;
}
