#include "ParentSystem.h"
#include "../component/Parent.h"
#include "../component/Transform.h"

void ParentSystem::onFrame(SceneManager& scene)
{
	auto& parentPool = scene.getPool<Parent>();
	auto& transformPool = scene.getPool<Transform>();

	for (auto& [id, parent] : parentPool)
	{
		auto& parent_tr = transformPool.getComponent(parent.parent);
		auto& tr = transformPool.getComponent(id);

		tr.orientationMatrix *= parent_tr.orientationMatrix;
	}
}
