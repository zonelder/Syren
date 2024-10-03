#pragma once
#include <DirectXMath.h>
#include "graphics/Drawable/BindComponent/constant_buffer.h"
#include "../component_serializer.h"



struct Transform
{
	/// @brief method for correct initialisation of Transform component 
	/// @param gfx 
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
	DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	DirectX::XMMATRIX orientationMatrix = DirectX::XMMatrixIdentity();

	VertexConstantBuffer<DirectX::XMMATRIX> vertexConstantBuffer;

};


SERIALIZE(Transform);