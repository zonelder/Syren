#include "pch.h"
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

Camera& SceneManager::getCamera() noexcept
{
	return _mainCamera;
}
