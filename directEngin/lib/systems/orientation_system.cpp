#include "orientation_system.h"
#include "../common/transform_utils.h"
#include "components/text_ui.h"
#include <iostream>


void OrientationSystem::onFrame(SceneManager& scene)
{
	 for (auto [entt,tr]:scene.view<Transform>())
	 {
	     tr.orientationMatrix.setTRS(tr.position,tr.rotation,tr.scale);

	 }
}
