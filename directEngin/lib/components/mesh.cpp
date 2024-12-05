#include "mesh.h"
#include "graphics/Drawable/BindComponent/vertex.h"


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

	float max = (std::numeric_limits<float>::max)();
	float min = (std::numeric_limits<float>::min)();
	auto& boundingBox = pMesh->boundingBox;
	boundingBox.maxBound = { min,min,min };
	boundingBox.minBound = { max,max, max };
	for (const auto& vert : pMesh->vertexes)//TODO include it to mesh i suppose.some dirty flag check:(
	{

		boundingBox.maxBound.x = (std::max)(boundingBox.maxBound.x, DirectX::XMVectorGetX(vert.position));
		boundingBox.maxBound.y = (std::max)(boundingBox.maxBound.y, DirectX::XMVectorGetY(vert.position));
		boundingBox.maxBound.z = (std::max)(boundingBox.maxBound.z, DirectX::XMVectorGetZ(vert.position));

		boundingBox.minBound.x = (std::min)(boundingBox.minBound.x, DirectX::XMVectorGetX(vert.position));
		boundingBox.minBound.y = (std::min)(boundingBox.minBound.y, DirectX::XMVectorGetY(vert.position));
		boundingBox.minBound.z = (std::min)(boundingBox.minBound.z, DirectX::XMVectorGetZ(vert.position));
	}
}
