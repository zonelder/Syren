#pragma once
#include "Mesh.h"
#include "Material.h"
#include <vector>
#include <string>
#include "../graphics/Drawable/BindComponent/BindableComponents.h"
#include <memory>

struct Render
{
	Mesh* p_mesh;
	std::shared_ptr<Material> p_material;
	Topology topology;
	bool is_rendered;

};
