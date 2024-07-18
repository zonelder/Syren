#include "meshPool.h"
#include "../../component/mesh.h"

Mesh* MeshPool::addMesh(Graphics& gfx,const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices, const MeshIternal::ConstantBuffer2& colors)
{
	meshes_.emplace_back(new  MeshIternal(gfx, vertices, indices, colors));
	wrappers_.emplace_back(meshes_.size()-1,std::size(indices));
	return &(wrappers_.back());
}

MeshIternal* MeshPool::getMesh(Mesh* mesh) const noexcept
{
	return meshes_[mesh->id].get();
}
