#include "ParentSystem.h"

void ParentSystem::setlGlobalMatrix(ComponentPool<Transform>& trPool, ComponentPool<Parent>& parentPool, EntityID curEntity)
{
	EntityID id = curEntity;

	Transform& main_tr = trPool.getComponent(id);
	Parent& main_p = parentPool.getComponent(id);
	DirectX::XMMATRIX globalMatrix =DirectX::XMMatrixIdentity() ;//main_tr.orientationMatrix;

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
	for (auto& entt : scene.getEntitiesWith<Parent, Transform>())
	{
		setlGlobalMatrix(transformPool, parentPool, entt.getID());
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
