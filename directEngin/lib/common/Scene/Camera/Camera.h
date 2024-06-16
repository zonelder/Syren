#pragma once
#include "../../TransformUtils.h"
#include "ClippingPlane.h"

class Camera
{
public:
	Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	void OnFrame();
	void OnUpdate();
	Transform transform;
	DirectX::XMMATRIX projection;

	/// @brief The Camera’s view angle, measured in radians
	float fov = 1.0f;


	/// @brief Proportions of the representation space X:Y.
	float aspectRatio = 1.0f;

	/// @brief 	Distances from the camera to start and stop rendering.
	/// @param far -  The furthest point relative to the camera that drawing will occur.
	/// @param near - The closest point relative to the camera that drawing will occur.
	ClippingPlanes clippingPlanes;

	/// @brief The color applied to the remaining screen after all elements in view have been drawn and there is no skybox
	float background[4] = {1.0f,1.0f, 1.0f, 1.0f};

};

