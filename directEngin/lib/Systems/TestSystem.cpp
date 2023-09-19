#include "TestSystem.h"
#include "../common/Input.h"


void TestSystem::onUpdate(SceneManager& scene, float time)
{
	float angle = time;
	auto& trPool = scene.getPool<Transform>();
	trPool.getComponent(0).rotation = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, angle, angle);
	angle = -angle;
	float x = 2.0f * Input::GetNormedX() - 1.0f;
	float y = 300;
	float z = 2.0f * Input::GetNormedY() - 1.0f;
	Transform& tr2 = trPool.getComponent(1);
	tr2.position.x = x;
	tr2.position.z = z;
	tr2.rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle, 0.0f, angle);
}
