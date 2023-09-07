#include "TransformUtils.h"

DirectX::XMMATRIX toOrientationMatrix(const Transform& tr)
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&tr.position);
	return DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&tr.scale), { 0.0f,0.0f,0.0f,0.0f }, tr.rotation, pos);
}