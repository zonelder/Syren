#pragma once
#include <string>
#include <vector>
#include <memory>

#include "math/vector3.h"
#include "math/color.h"
#include "graphics/Drawable/BindComponent/vertex.h"

struct BoundingBox
{
	using Position = Vector3;
	Position minBound;
	Position maxBound;
};

struct Mesh
{
	//in case i will implement more catch friendly container
	template<class T>
	using ElementContainer = std::vector<T>;
public:
	using Position_t = Vertex;
	using UV_t = DirectX::XMFLOAT2;
	using Normal_t = Vector3;
	using Color_t = Color;
	using Index_t = unsigned short;

	Mesh() = default;
	Mesh(ElementContainer<Vertex>&& verts, ElementContainer<Index_t>&& inds, ElementContainer<Color_t>&& colors);
	std::string resourceID;
	std::string identifier;
	size_t IndexCount;
	size_t startIndex;

	ElementContainer<Vertex> vertexes;
	ElementContainer<UV_t> uvs;
	ElementContainer< Vector3 > normals;
	ElementContainer<Index_t> indices;
	ElementContainer< Color_t > colors;
	BoundingBox boundingBox;

};

using MeshPtr = std::shared_ptr<Mesh>;

namespace meshHelpers
{
	void updateBB(Mesh* pMesh);
}