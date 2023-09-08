#pragma once
#include <vector>

struct Mesh
{
	std::vector<DirectX::XMVECTOR> vertices;
	std::vector<unsigned short> indices;
	struct ConstantBuffer2 {
		struct {
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	ConstantBuffer2 colors;
};