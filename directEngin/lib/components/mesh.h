#pragma once
#include <string>
#include <vector>

struct Color
{
	float r{ 0.0f };
	float g{ 0.0f };
	float b{ 0.0f };
	float a{ 1.0f };
};

struct Mesh
{
	//in case i will implement more catch friendly container
	template<class T>
	using ElementContainer = std::vector<T>;
public:
	Mesh() = default;
	Mesh(size_t id, size_t MeshSize);
	std::string resourceID;
	size_t IndexCount;
	size_t startIndex;

	ElementContainer<Vertex> vertexes;
	ElementContainer<unsigned short> indices;
	ElementContainer< Color > colors;
};