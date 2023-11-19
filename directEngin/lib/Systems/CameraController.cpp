#include "CameraController.h"
#include "../common/Input.h"
#include "../component/TimedRotation.h"

void CameraController::onUpdate(SceneManager& scene, float time)
{
	float speed = 0.01f;
	Transform& cameraTr = scene.getCamera().transform;
	if (Input::LeftKeyDown())// moving in left mouse button pressed
	{
		cameraTr.position.x -= speed * Input::GetDeltaX();
		cameraTr.position.y += speed * Input::GetDeltaY();
		cameraTr.position.z = -4.0f;
		return;
	}
	if (true && Input::RightKeyDown())// rotating in left mouse button pressed
	{
		float cam_yaw = -speed * Input::GetDeltaX();
		float cam_pitch = -speed * Input::GetDeltaY();
		cameraTr.rotation = DirectX::XMQuaternionMultiply(cameraTr.rotation, DirectX::XMQuaternionRotationRollPitchYaw(cam_pitch, cam_yaw, 0.0f));
		return;
	}

	if (Input::IsKeyDown('Q'))
	{


	}
}
