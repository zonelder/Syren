#pragma once

struct Mesh
{
public:
	Mesh(size_t id, size_t MeshSize);
	size_t id;
	size_t IndexCount;
	size_t startIndex;
};