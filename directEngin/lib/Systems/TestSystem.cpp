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
	float speed = 0.01f;
	Transform& cameraTr = scene.getCamera().transform;
	if (Input::LeftKeyDown())// moving in left mouse button pressed
	{
		cameraTr.position.x -= speed * Input::GetDeltaX();
		cameraTr.position.y += speed * Input::GetDeltaY();
		cameraTr.position.z = -4.0f;
		return;
	}
	if (false && Input::RightKeyDown())// rotating in left mouse button pressed
	{
		float cam_yaw = -speed * Input::GetDeltaX();
		float cam_pitch = -speed * Input::GetDeltaY();
		cameraTr.rotation = DirectX::XMQuaternionMultiply(cameraTr.rotation, DirectX::XMQuaternionRotationRollPitchYaw(cam_pitch, cam_yaw, 0.0f));
		return;
	}
}
