#include "TimedRotationtSystem.h"
#include "../component/TimedRotation.h"
#include "../common/Input.h"


void TimedRotationSystem::onUpdate(SceneManager& scene, float time)
{
	float angle = time;
	auto& trPool = scene.getPool<Transform>();

	auto& rotPool = scene.getPool<TimedRotation>();

	for (auto& [entID, rot] : rotPool)
	{
		if (!trPool.hasComponent(entID))
			continue;
		trPool.getComponent(entID).rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle*rot.xSence, angle*rot.ySence, angle*rot.zSence);

	}
}
