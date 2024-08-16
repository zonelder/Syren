#pragma once
#include <vector>
#include <memory.h>
#include "components/mesh_iternal.h"


using meshPtrVector = std::vector<std::shared_ptr<MeshIternal>>;

struct Mesh;


class MeshPool
{
public:


	/// @brief create new mesh and return ptr to it
	/// @return 
	Mesh* addMesh(
		Graphics& gfx,
		const std::vector<Vertex>& vertices,
		const std::vector<unsigned short>& indices,
		const MeshIternal::ConstantBuffer2& colors
	);

	MeshIternal* getMesh(Mesh* mesh) const noexcept;

private:

	meshPtrVector meshes_;
	std::vector<Mesh> wrappers_;
};