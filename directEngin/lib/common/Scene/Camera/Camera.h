#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "components/transform.h"
#include "math/color.h"

class Camera
{
public:

	Camera();
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	void OnFrame();
	void OnUpdate();

	const Matrix4x4& view() const noexcept { return _view; }

	const Matrix4x4& projection() const noexcept { return _projection; }

	Transform transform;

	/// @brief The Camera’s view angle, measured in radians
	float fov;

	/// @brief Proportions of the representation space X:Y.
	float aspectRatio;

	/// @brief near - The closest distance relative to the camera that drawing will occur.
	float nearPlane;

	/// @brief farPlane -  The furthest distance relative to the camera that drawing will occur.
	float farPlane;

	/// @brief The color applied to the remaining screen after all elements in view have been drawn and there is no skybox
	Color background = Color::white;

private:
	Matrix4x4 _view;
	Matrix4x4 _projection;

};

#endif // !__CAMERA_H_