#pragma once
#include <vector>
#include <DirectXMath.h>
#include "../graphics/Drawable/BindComponent/vertex_buffer.h"
#include "../graphics/Drawable/BindComponent/index_buffer.h"
#include "../graphics/Drawable/BindComponent/constant_buffer.h"
#include "../graphics/Drawable/BindComponent/vertex.h"

struct BoundingBox
{
	using Position = DirectX::XMFLOAT3;
	Position minBound;
	Position maxBound;
};


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
	BoundingBox boundingBox;
	unsigned int id;
private:
	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	PixelConstantBuffer<ConstantBuffer2> _pixelConstantBuffer;


};