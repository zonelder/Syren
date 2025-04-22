#include "pch.h"
#include "scene_manager.h"
#include "components/parent.h"

const Entity& SceneManager::createEntity() noexcept
{
	return _entityManager.create();
}

bool SceneManager::destroy(const Entity& entt) noexcept
{
	return destroy(entt.getID());
}

bool SceneManager::destroy(EntityID entt_id) noexcept
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

const Entity& SceneManager::getEntity(EntityID id) const noexcept
{
	return _entityManager.get(id);
}

Camera& SceneManager::getCamera() noexcept
{
	return _mainCamera;
}
