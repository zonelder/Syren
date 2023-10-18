#include "Mesh.h"



Mesh::Mesh(Graphics& gfx, const std::vector<DirectX::XMVECTOR>& Verts, const std::vector<unsigned short>& Indices, const ConstantBuffer2& Colors)
{
	vertices = Verts;
	indices = Indices;
	colors = Colors;

	_vertexBuffer = VertexBuffer(gfx, vertices);
	_indexBuffer = IndexBuffer(gfx, indices);
	_pixelConstantBuffer = PixelConstantBuffer<Mesh::ConstantBuffer2>(gfx, colors);
}

void Mesh::bind(Graphics& gfx)
{
	_vertexBuffer.bind(gfx);
	_indexBuffer.bind(gfx);
	_pixelConstantBuffer.bind(gfx);
}