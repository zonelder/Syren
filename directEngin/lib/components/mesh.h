#pragma once
#include <string>
#include <vector>
#include <memory>

#include <DirectXMath.h>
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
	using Position = DirectX::XMFLOAT3;
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
	ElementContainer<unsigned short> indices;
	ElementContainer< Color > colors;
	BoundingBox boundingBox;
};

using MeshPtr = std::shared_ptr<Mesh>;

namespace meshHelpers
{
	void updateBB(Mesh* pMesh);
}