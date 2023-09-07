#pragma once
#include <DirectXMath.h>

struct Transform
{
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
	DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
};
