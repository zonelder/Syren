#include "parent_system.h"

void ParentSystem::setlGlobalMatrix(ComponentPool<Transform>& trPool, ComponentPool<Parent>& parentPool, EntityID curEntity)
{
	EntityID id = curEntity;

	Transform& main_tr = trPool.getComponent(id);
	Parent& main_p = parentPool.getComponent(id);
	Matrix4x4 globalMatrix;

	while (parentPool.hasComponent(id))
	{

		auto& pComponent = parentPool.getComponent(id);
		auto& pTr = trPool.getComponent(pComponent.parent);

		if (pComponent.used)
		{
			break;
		}
		globalMatrix = globalMatrix* pTr.orientationMatrix;
		id = pComponent.parent;
	}

	main_tr.orientationMatrix *= globalMatrix;

	main_p.used = true;

}

void ParentSystem::onFrame(SceneManager& scene)
{
	auto& parentPool = scene.getPool<Parent>();
	auto& transformPool = scene.getPool<Transform>();
	for (auto [entt,r,tr] : scene.view<Parent, Transform>())
	{
		setlGlobalMatrix(transformPool, parentPool, entt);
	}
}


void ParentSystem::onUpdate(SceneManager& scene,float dt)
{
	auto& pool = scene.getPool<Parent>();
	for (auto& parent : pool)
	{
		parent.used = false;
	}
}
