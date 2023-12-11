#include "OrientationSystem.h"
#include "../common/TransformUtils.h"


void OrientationSystem::onFrame(SceneManager& scene)
{
	ComponentPool<Transform>& _transforms = scene.getPool<Transform>();

	for (auto& [entID, tr] : _transforms)
	{
		tr.orientationMatrix = toOrientationMatrix(tr);
	}

}
