#include "TimedRotationtSystem.h"
#include "../component/TimedRotation.h"
#include "../common/Input.h"


void TimedRotationSystem::onUpdate(SceneManager& scene, float time)
{
	float angle = time;
	auto& trPool = scene.getPool<Transform>();

	auto& rotPool = scene.getPool<TimedRotation>();

	for (auto& entt : scene.getEntitiesWith<TimedRotation, Transform>())
	{

		auto& rot = scene.getComponent<TimedRotation>(entt);
		auto& tr = scene.getComponent<Transform>(entt);
		tr.rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle*rot.xSence, angle*rot.ySence, angle*rot.zSence);
	}
}
