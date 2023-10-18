#pragma once
#include <vector>
#include <DirectXMath.h>
#include "../graphics/Drawable/BindComponent/VertexBuffer.h"
#include "../graphics/Drawable/BindComponent/IndexBuffer.h"
#include "../graphics/Drawable/BindComponent/ConstantBuffer.h"

struct Mesh
{

	void init(Graphics& gfx);
	void bind(Graphics& gfx);
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

	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	PixelConstantBuffer<ConstantBuffer2> pixelConstantBuffer;

private:
	bool isInit;

};