#pragma once
#include <string>
#include <vector>
#include <memory>

#include "math/vector3.h"
#include "graphics/Drawable/BindComponent/vertex.h"

struct Color
{
	float r{ 0.0f };
	float g{ 0.0f };
	float b{ 0.0f };
	float a{ 1.0f };
};

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
	Mesh() = default;
	Mesh(ElementContainer<Vertex>&& verts, ElementContainer<unsigned short>&& inds, ElementContainer<Color>&& colors);
	std::string resourceID;
	std::string identifier;
	size_t IndexCount;
	size_t startIndex;

	ElementContainer<Vertex> vertexes;
	ElementContainer< DirectX::XMFLOAT2> uvs;
	ElementContainer< Vector3 > normals;
	ElementContainer<unsigned short> indices;
	ElementContainer< Color > colors;
	BoundingBox boundingBox;

	using Position_t = Vertex;
	using UV_t = DirectX::XMFLOAT2;
	using Normal_t = Vector3;
	using Color_t = Color;
	using Index_t = unsigned short;
};

using MeshPtr = std::shared_ptr<Mesh>;

namespace meshHelpers
{
	void updateBB(Mesh* pMesh);
}