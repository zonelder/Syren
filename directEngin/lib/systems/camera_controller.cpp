#include "camera_controller.h"
#include "../common/Input.h"
#include "components/timed_rotation.h"
#include <cmath>


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
	DirectX::XMFLOAT3 movement = { 0.0f, 0.0f, 0.0f };
	if (input.IsKeyDown('W')) movement.z = + 1.0f;

	if (input.IsKeyDown('S')) movement.z = -1.0f;

	if (input.IsKeyDown('A')) movement.x = -1.0f;
	if (input.IsKeyDown('D')) movement.x = +1.0f;

	if (movement.x != 0.0f || movement.y != 0.0f || movement.z != 0.0f)
	{
		// Get camera's local axes
		DirectX::XMVECTOR rotation = cameraTr.rotation;
		DirectX::XMVECTOR forward = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotation);
		DirectX::XMVECTOR right = DirectX::XMVector3Rotate(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotation);
		DirectX::XMVECTOR up = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotation);

		// Calculate movement vector in world space
		DirectX::XMVECTOR moveVector = DirectX::XMVectorAdd(DirectX::XMVectorScale(right, movement.x), DirectX::XMVectorScale(up, movement.y));

		moveVector = DirectX::XMVectorAdd(moveVector, DirectX::XMVectorScale(forward, movement.z));
		moveVector = DirectX::XMVector3Normalize(moveVector);
		// Apply speed and delta time
		moveVector = DirectX::XMVectorScale(moveVector, movementSpeed * time);

		// Update camera position
		DirectX::XMVECTOR currentPos = DirectX::XMLoadFloat3(&cameraTr.position);
		currentPos = DirectX::XMVectorAdd(currentPos, moveVector);
		DirectX::XMStoreFloat3(&cameraTr.position, currentPos);
	}
	
}
