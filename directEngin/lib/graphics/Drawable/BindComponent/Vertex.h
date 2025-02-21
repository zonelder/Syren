#pragma once
#include "IBindable.h"

struct Vertex
{
	DirectX::XMVECTOR position;
	DirectX::XMFLOAT2 uv;
};


struct VertexXYZD
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR color;
};

