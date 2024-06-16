#include "CameraController.h"
#include "../common/Input.h"
#include "../component/TimedRotation.h"
#include <cmath>

float sgn(float val)
{
	if (val >= 0) return 1.0f;
	return -1.0f;
}

void CameraController::onUpdate(SceneManager& scene, float time)
{
	float speed = 0.01f;
	Transform& cameraTr = scene.getCamera().transform;
	const auto& input = scene.getInput();
	bool left_pressed = input.isLeftPressed;
	/*
	for (auto& entt : scene.getEntitiesWith<Render>())
	{
		auto mat = scene.getComponent<Render>(entt).p_material;
		mat->color.x = sinf(time);
		mat->color.z = 1 - mat->color.x;
		break;
	}
	*/
	if (left_pressed)// moving in left mouse button pressed
	{
		float dx = speed * input.deltaX;
		float dy = -speed * input.deltaY;
		cameraTr.position.x -= dx;
		cameraTr.position.y -= dy;

		return;
	}
	if (scene.getInput().isRightPressed)
	{
		scene.destroyEntity(0);
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
