#pragma once
#include <vector>
#include "../graphics/Drawable/BindComponent/Vertex.h"


struct Vertex;
using Vertexes = std::vector<Vertex>;
using Indixies = std::vector<unsigned short>;

class Geometry
{
public:

	static void boxVertex(Vertexes& verts) noexcept;

	static void boxIndixes(Indixies& indices,size_t offset = 0u) noexcept;

};