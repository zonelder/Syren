#include "TimedRotationtSystem.h"
#include "../component/TimedRotation.h"
#include "../common/Input.h"


void TimedRotationSystem::onUpdate(SceneManager& scene, float time)
{
	float angle = time;
	auto& trPool = scene.getPool<Transform>();

	auto& rotPool = scene.getPool<TimedRotation>();

	for (auto [entt,rot,tr]:scene.view<TimedRotation, Transform>())
	{
		tr.rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle*rot.xSence, angle*rot.ySence, angle*rot.zSence);
	}
}
