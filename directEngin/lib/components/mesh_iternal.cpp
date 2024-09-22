#include "mesh_iternal.h"
#include <algorithm>
#include <limits>
///vertex= {DirectX::XMVECTOR - cord,float2 - UV_CORD}


MeshIternal::MeshIternal(Graphics& gfx, const std::vector<Vertex>& Verts, const std::vector<unsigned short>& Indices, const ConstantBuffer2& Colors)
{
	vertices = Verts;
	indices = Indices;
	colors = Colors;

	_vertexBuffer = VertexBuffer(gfx, vertices);
	_indexBuffer = IndexBuffer(gfx, indices);
	_pixelConstantBuffer = PixelConstantBuffer<MeshIternal::ConstantBuffer2>(gfx, colors);
	float max = (std::numeric_limits<float>::max)();
	float min = (std::numeric_limits<float>::min)();

	boundingBox.maxBound = { min,min,min };
	boundingBox.minBound = { max,max, max };
	for (const auto& vert : Verts)
	{

		boundingBox.maxBound.x = (std::max)(boundingBox.maxBound.x,DirectX::XMVectorGetX(vert.position));
		boundingBox.maxBound.y = (std::max)(boundingBox.maxBound.y, DirectX::XMVectorGetY(vert.position));
		boundingBox.maxBound.z = (std::max)(boundingBox.maxBound.z, DirectX::XMVectorGetZ(vert.position));

		boundingBox.minBound.x = (std::min)(boundingBox.minBound.x, DirectX::XMVectorGetX(vert.position));
		boundingBox.minBound.y = (std::min)(boundingBox.minBound.y, DirectX::XMVectorGetY(vert.position));
		boundingBox.minBound.z = (std::min)(boundingBox.minBound.z, DirectX::XMVectorGetZ(vert.position));
	}
}

void MeshIternal::bind(Graphics& gfx)
{
	_vertexBuffer.bind(gfx);
	_indexBuffer.bind(gfx);
	//_pixelConstantBuffer.bind(gfx);
}