#include "camera_controller.h"
#include "../common/Input.h"
#include "components/timed_rotation.h"
#include <cmath>


void CameraController::onUpdate(SceneManager& scene, float time)
{
	float speed = 0.01f;
	Transform& cameraTr = scene.getCamera().transform;
	const auto& input = scene.getInput();
	bool left_pressed = input.isLeftPressed;
	if (left_pressed)// moving in left mouse button pressed
	{
		float dx = speed * input.deltaX;
		float dy = -speed * input.deltaY;
		cameraTr.position.x -= dx;
		cameraTr.position.y -= dy;

		return;
	}
	if (input.isRightPressed)// rotating in left mouse button pressed
	{
		float cam_yaw = speed * input.deltaX;
		float cam_pitch = speed * input.deltaY;

		// Calculate yaw quaternion
		DirectX::XMVECTOR yawQuat = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), cam_yaw);

		// Calculate the forward vector from the current rotation
		DirectX::XMVECTOR forwardVector = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), cameraTr.rotation);

		// Calculate the right vector
		DirectX::XMVECTOR rightVector = DirectX::XMVector3Cross(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), forwardVector);
		auto len = DirectX::XMVectorGetX(DirectX::XMVector3Length(rightVector));
		if(len > 0.001f)
			rightVector = DirectX::XMVector3Normalize(rightVector);

		// Calculate pitch quaternion
		DirectX::XMVECTOR pitchQuat = DirectX::XMQuaternionRotationAxis(rightVector, cam_pitch);

		// Combine the yaw and pitch quaternions
		cameraTr.rotation = DirectX::XMQuaternionMultiply(cameraTr.rotation, pitchQuat);
		cameraTr.rotation = DirectX::XMQuaternionMultiply(cameraTr.rotation, yawQuat);

		// Normalize the quaternion to prevent drift
		cameraTr.rotation = DirectX::XMQuaternionNormalize(cameraTr.rotation);
		return;
	}
}
