#include "camera_controller.h"
#include "../common/Input.h"
#include "components/timed_rotation.h"
#include <cmath>


void CameraController::onUpdate(SceneManager& scene, float time)
{
	float speed = 1.0f;
	float movementSpeed = 0.1f;
	Transform& cameraTr = scene.getCamera().transform;
	const auto& input = context::input();
	if (!input.isRightPressed)// rotating in left mouse button pressed
		return;
	float cam_yaw = speed * input.deltaX;
	float cam_pitch = speed * input.deltaY;
	Quaternion yawQuat = Quaternion::angleAxis(cam_yaw, Vector3::up);

	// Calculate the forward vector from the current rotation
	Vector3 forwardVector = cameraTr.rotation * Vector3::forward;
	Vector3 rightVector = Vector3::up.cross(forwardVector);
	rightVector.normalize();

	Quaternion pitchQuat = Quaternion::angleAxis(cam_pitch, rightVector);

	cameraTr.rotation *= pitchQuat * yawQuat;

	// Normalize the quaternion to prevent drift
	//cameraTr.rotation = DirectX::XMQuaternionNormalize(cameraTr.rotation);

	// check movving
	Vector3 movement = { 0.0f, 0.0f, 0.0f };
	if (input.IsKeyDown('W')) movement[2] = +1.0f;

	if (input.IsKeyDown('S')) movement[2] = -1.0f;

	if (input.IsKeyDown('A')) movement[0] = -1.0f;
	if (input.IsKeyDown('D')) movement[0] = +1.0f;

	if (movement[0] != 0.0f || movement[1] != 0.0f || movement[2] != 0.0f)
	{
		// Get camera's local axes
		const auto& rotation = cameraTr.rotation;

		Vector3 forward = rotation * Vector3::forward;;
		Vector3 right = rotation*Vector3::right;
		Vector3 up = rotation*Vector3::up;

		// Calculate movement vector in world space
		Vector3 moveVector = right * movement[0] + up * movement[1] + forward * movement[2];
		moveVector.normalize();
		moveVector *= movementSpeed * time;
		cameraTr.position += moveVector;
	}
	
}
