#include "camera_controller.h"
#include "../common/Input.h"
#include "components/timed_rotation.h"
#include <cmath>
#include "math/vector3.h"


void CameraController::onUpdate(SceneManager& scene, float time)
{
	float speed = 0.01f;
	float movementSpeed = 0.1f;
	Transform& cameraTr = scene.getCamera().transform;
	const auto& input = context::input();
	if (!input.isRightPressed)// rotating in left mouse button pressed
		return;
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

	// check movving
	Vector3 movement = { 0.0f, 0.0f, 0.0f };
	if (input.IsKeyDown('W')) movement[2] = +1.0f;

	if (input.IsKeyDown('S')) movement[2] = -1.0f;

	if (input.IsKeyDown('A')) movement[0] = -1.0f;
	if (input.IsKeyDown('D')) movement[0] = +1.0f;

	if (movement[0] != 0.0f || movement[1] != 0.0f || movement[2] != 0.0f)
	{
		// Get camera's local axes
		DirectX::XMVECTOR rotation = cameraTr.rotation;
		Vector3 forward(DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotation));
		Vector3 right(DirectX::XMVector3Rotate(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotation));
		Vector3 up(DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotation));

		// Calculate movement vector in world space
		Vector3 moveVector = right * movement[0] + up * movement[1] + forward * movement[2];
		moveVector.normalize();
		moveVector *= movementSpeed * time;
		cameraTr.position += moveVector;
	}
	
}
