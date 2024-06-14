#pragma once
#include <vector>
#include <DirectXMath.h>
#include "../graphics/Drawable/BindComponent/VertexBuffer.h"
#include "../graphics/Drawable/BindComponent/IndexBuffer.h"
#include "../graphics/Drawable/BindComponent/ConstantBuffer.h"
#include "../graphics/Drawable/BindComponent/Vertex.h"


struct MeshIternal
{
	struct ConstantBuffer2 {
		struct {
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	MeshIternal(Graphics& gfx,const std::vector<Vertex>& verts,const std::vector<unsigned short>& indices,const ConstantBuffer2& colors);
	void bind(Graphics& gfx);
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	ConstantBuffer2 colors;
	unsigned int id;
private:
	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	PixelConstantBuffer<ConstantBuffer2> _pixelConstantBuffer;

};