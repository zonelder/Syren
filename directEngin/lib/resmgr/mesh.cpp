#include "pch.h"

#include "mesh.h"
#include "graphic/vertex.h"

#undef max
#undef min

Mesh::Mesh(ElementContainer<Vertex>&& verts, ElementContainer<unsigned short>&& inds, ElementContainer<Color>&& colors) : 
	vertexes(std::move(verts)),
	indices(std::move(inds)),
	colors(std::move(colors)),
	IndexCount(indices.size()),
	startIndex(0u)
{
	meshHelpers::updateBB(this);
}

void meshHelpers::updateBB(Mesh* pMesh)
{
	if (!pMesh)
		return;

	float max = std::numeric_limits<float>::max();
	float min = std::numeric_limits<float>::min();
	auto& boundingBox = pMesh->boundingBox;
	boundingBox.maxBound = { min,min,min };
	boundingBox.minBound = { max,max, max };
	for (const auto& vert : pMesh->vertexes)//TODO include it to mesh i suppose.some dirty flag check:(
	{
		boundingBox.maxBound = Vector3::max(boundingBox.maxBound, vert.position);
		boundingBox.minBound = Vector3::min(boundingBox.minBound, vert.position);
	}
}
