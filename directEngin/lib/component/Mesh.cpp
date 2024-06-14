#include "mesh.h"

Mesh::Mesh(size_t id_, size_t MeshSize_)
	:
	id(id_),
	startIndex(0u),
	IndexCount(MeshSize_)
{
}
