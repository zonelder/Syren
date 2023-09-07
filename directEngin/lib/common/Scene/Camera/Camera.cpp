#include "Camera.h"



void Camera::OnFrame()
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&transform.position);
	orientationMatrix =
		DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&transform.scale), pos, transform.rotation, DirectX::XMVectorNegate(pos)) *
		DirectX::XMMatrixPerspectiveFovLH(
			fov,
			aspectRatio,
			clippingPlanes.near,
			clippingPlanes.far
		);

}
