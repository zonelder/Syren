#include "OrientationSystem.h"
#include "../common/TransformUtils.h"
#include "../component/TextUI.h"
#include <iostream>


void OrientationSystem::onFrame(SceneManager& scene)
{
	ComponentPool<Transform>& _transforms = scene.getPool<Transform>();

	 for (auto& entt : scene.getEntitiesWith<Transform>())
	 {


	     auto& tr = scene.getComponent<Transform>(entt);
	     tr.orientationMatrix = toOrientationMatrix(tr);

	 }
}
