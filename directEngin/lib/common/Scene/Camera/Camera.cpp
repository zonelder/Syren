#include "Camera.h"
#include <iostream>
#undef near
#undef far

const double PI = acos(-1.0);

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
