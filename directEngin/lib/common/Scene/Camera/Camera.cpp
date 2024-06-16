#include "Camera.h"
#include <iostream>
#undef near
#undef far

const double PI = acos(-1.0);

void Camera::OnFrame()
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&transform.position);
	projection = DirectX::XMMatrixPerspectiveFovLH(
		fov,
		aspectRatio,
		clippingPlanes.near,
		clippingPlanes.far
	);
	transform.orientationMatrix =
		DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&transform.scale), pos, transform.rotation, DirectX::XMVectorNegate(pos)) * projection;


}


void Camera::OnUpdate()
{
}
