#include "ParentSystem.h"

void ParentSystem::setlGlobalMatrix(ComponentPool<Transform>& trPool, ComponentPool<Parent>& parentPool, EntityID curEntity)
{
	EntityID id = curEntity;

	// if parent is not set then we dont have to modify transform. 
	if (!(parentPool.hasComponent(id)))
	{
		return;
	}

	Transform& main_tr = trPool.getComponent(id);
	Parent& main_p = parentPool.getComponent(id);
	DirectX::XMMATRIX globalMatrix =DirectX::XMMatrixIdentity() ;//main_tr.orientationMatrix;

	int tree = id;
	while (parentPool.hasComponent(id))
	{

		auto& pComponent = parentPool.getComponent(id);
		auto& pTr = trPool.getComponent(pComponent.parent);

		if (pComponent.used)
		{
			break;
		}
		globalMatrix = globalMatrix* pTr.orientationMatrix;
		tree = tree * 10 + pComponent.parent;
		id = pComponent.parent;
	}

	main_tr.orientationMatrix *= globalMatrix;

	main_p.used = true;

}

void ParentSystem::onFrame(SceneManager& scene)
{
	auto& parentPool = scene.getPool<Parent>();
	auto& transformPool = scene.getPool<Transform>();

	for (auto& [id, parent] : parentPool)
	{
		setlGlobalMatrix(transformPool, parentPool, id);//TODO avoid recalculate entities that already been calculated in prev step
	}
}


void ParentSystem::onUpdate(SceneManager& scene,float dt)
{
	for (auto& [id, parent] : scene.getPool<Parent>())
	{
		parent.used = false;
	}
}
