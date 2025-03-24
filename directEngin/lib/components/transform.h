#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include "serialization/base.h"
#include "math/vector3.h"

SERIALIZE
struct Transform
{
	SERIALIZE_FIELD
	Vector3 position{ 0.0f,0.0f,0.0f };

	SERIALIZE_FIELD
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
	
	SERIALIZE_FIELD
	Vector3 scale{ 1.0f,1.0f,1.0f };

	DirectX::XMMATRIX orientationMatrix = DirectX::XMMatrixIdentity();

};


#endif