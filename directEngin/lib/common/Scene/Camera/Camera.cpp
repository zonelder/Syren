#include "Camera.h"
#undef near
#undef far


Camera::Camera() :
	farPlane(500.0f),
	nearPlane(0.15f),
	fov(45.0f),
	aspectRatio(1.0f)
{}

void Camera::OnFrame()
{
	_projection = Matrix4x4::perspective( fov,aspectRatio, nearPlane,farPlane );
	transform.orientationMatrix.setTRS(transform.position, transform.rotation, transform.scale);
	_view = transform.orientationMatrix.inverse();

}

void Camera::OnUpdate()
{
}
