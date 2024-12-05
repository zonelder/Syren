#include "mesh_iternal.h"
#include <algorithm>
#include <limits>


MeshInternal::MeshInternal(Graphics& gfx,Mesh* pMesh) : _isReady(false)
{
	if (!pMesh)
		return;
	_isReady = true;
	_vertexBuffer = VertexBuffer(gfx,pMesh->vertexes);
	_indexBuffer = IndexBuffer(gfx,pMesh->indices);
	_pixelConstantBuffer =decltype(_pixelConstantBuffer)(gfx,pMesh->colors);

}

void MeshInternal::bind(Graphics& gfx)
{
	if (!_isReady)
		return;

	_vertexBuffer.bind(gfx);
	_indexBuffer.bind(gfx);
	//_pixelConstantBuffer.bind(gfx);
}