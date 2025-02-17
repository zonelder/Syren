#include "Camera.h"
#include <iostream>
#undef near
#undef far

const double PI = acos(-1.0);

Camera::Camera() :
	farPlane(500.0f),
	nearPlane(0.15f),
	fov(1.0f),
	aspectRatio(1.0f)
{}

void Camera::OnFrame()
{
	auto pos = DirectX::XMLoadFloat3(&transform.position);
	auto scale = DirectX::XMLoadFloat3(&transform.scale);
	_projection = DirectX::XMMatrixPerspectiveFovLH( fov,aspectRatio,farPlane,nearPlane );
	transform.orientationMatrix =
		DirectX::XMMatrixAffineTransformation(scale, DirectX::XMVectorZero(), transform.rotation, pos);
	_view = DirectX::XMMatrixInverse(nullptr ,transform.orientationMatrix);

}

void Camera::OnUpdate()
{
}
