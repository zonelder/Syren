#include "transform_utils.h"

DirectX::XMMATRIX toOrientationMatrix(const Transform& tr)
{
	return DirectX::XMMatrixAffineTransformation(tr.scale, { 0.0f,0.0f,0.0f,0.0f }, tr.rotation, tr.position);
}