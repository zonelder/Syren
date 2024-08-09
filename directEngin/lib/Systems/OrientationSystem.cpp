#include "OrientationSystem.h"
#include "../common/TransformUtils.h"
#include "../component/TextUI.h"
#include <iostream>


void OrientationSystem::onFrame(SceneManager& scene)
{
	 for (auto [entt,tr]:scene.view<Transform>())
	 {
	     tr.orientationMatrix = toOrientationMatrix(tr);

	 }
}
