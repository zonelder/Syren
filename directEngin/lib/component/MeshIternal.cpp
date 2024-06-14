#include "MeshIternal.h"
///vertex= {DirectX::XMVECTOR - cord,float2 - UV_CORD}


MeshIternal::MeshIternal(Graphics& gfx, const std::vector<Vertex>& Verts, const std::vector<unsigned short>& Indices, const ConstantBuffer2& Colors)
{
	vertices = Verts;
	indices = Indices;
	colors = Colors;

	_vertexBuffer = VertexBuffer(gfx, vertices);
	_indexBuffer = IndexBuffer(gfx, indices);
	_pixelConstantBuffer = PixelConstantBuffer<MeshIternal::ConstantBuffer2>(gfx, colors);
}

void MeshIternal::bind(Graphics& gfx)
{
	_vertexBuffer.bind(gfx);
	_indexBuffer.bind(gfx);
	//_pixelConstantBuffer.bind(gfx);
}