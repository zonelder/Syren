#pragma once
#include <DirectXMath.h>
#include "graphics/Drawable/BindComponent/constant_buffer.h"
#include "../serialization/component_serializer.h"


SERIALIZE
struct Transform
{
	SERIALIZE_FIELD
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };

	SERIALIZE_FIELD
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
	
	SERIALIZE_FIELD
	DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	DirectX::XMMATRIX orientationMatrix = DirectX::XMMatrixIdentity();

	VertexConstantBuffer<DirectX::XMMATRIX> vertexConstantBuffer;

};