#pragma once
#include <vector>
#include <DirectXMath.h>

#include "components/mesh.h"
#include "../graphics/Drawable/BindComponent/vertex_buffer.h"
#include "../graphics/Drawable/BindComponent/index_buffer.h"
#include "../graphics/Drawable/BindComponent/constant_buffer.h"
#include "../graphics/Drawable/BindComponent/vertex.h"


struct MeshInternal
{
	MeshInternal(Graphics& gfx,Mesh* pMesh);
	void bind(Graphics& gfx);
	BoundingBox boundingBox;
private:
	bool _isReady;
	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	PixelConstantBuffer<std::vector<Color>> _pixelConstantBuffer;


};