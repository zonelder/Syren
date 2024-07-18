#include "Geometry.h"

void Geometry::boxVertex(Vertexes& verts) noexcept
{
	constexpr size_t boxVertSize = 8;
	auto size = std::size(verts);
	if (verts.capacity() < size + boxVertSize)
	{
		verts.resize(size + boxVertSize);
	}

	verts.push_back({ { -1.0f,-1.0f,0.0f,},{0.0f,1.0f} });
	verts.push_back({ { -1.0f,-1.0f,0.0f,},{0.0f,0.0f} });
	verts.push_back({ { -1.0f,-1.0f,0.0f,},{1.0f,0.0f} });
	verts.push_back({ { -1.0f,-1.0f,0.0f,},{1.0f,1.0f} });


	verts.push_back({ { -1.0f,-1.0f,1.0f,},{0.0f,1.0f} });
	verts.push_back({ { -1.0f,-1.0f,1.0f,},{0.0f,0.0f} });
	verts.push_back({ { -1.0f,-1.0f,1.0f,},{1.0f,0.0f} });
	verts.push_back({ { -1.0f,-1.0f,1.0f,},{1.0f,1.0f} });

}

void Geometry::boxIndixes(Indixies& indices, size_t offset) noexcept
{
	//no imp

}
