#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include "serialization/base.h"
#include "math/vector3.h"
#include "math/quaternion.h"

SERIALIZE
struct Transform
{
	SERIALIZE_FIELD
	Vector3 position;

	SERIALIZE_FIELD
	Quaternion rotation;
	
	SERIALIZE_FIELD
	Vector3 scale{ 1.0f,1.0f,1.0f };

	DirectX::XMMATRIX orientationMatrix = DirectX::XMMatrixIdentity();

};


#endif