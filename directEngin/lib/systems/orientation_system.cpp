#include "orientation_system.h"

void OrientationSystem::onFrame(SceneManager& scene)
{
	 for (auto [entt,tr]:scene.view<Transform>())
	 {
	     tr.orientationMatrix.setTRS(tr.position,tr.rotation,tr.scale);

	 }
}
