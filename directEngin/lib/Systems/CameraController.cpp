#include "CameraController.h"
#include "../common/Input.h"
#include "../component/TimedRotation.h"

void CameraController::onUpdate(SceneManager& scene, float time)
{
	float speed = 0.01f;
	Transform& cameraTr = scene.getCamera().transform;
	bool left_pressed = Input::LeftKeyDown();
	if (left_pressed)// moving in left mouse button pressed
	{
		float dx = speed * Input::GetDeltaX();
		float dy = -speed * Input::GetDeltaY();
		cameraTr.position.x -= dx;

		cameraTr.position.y -= dy;

		return;
	}
	/*
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
	*/
}
