#pragma once
#include "Mesh.h"
#include "Material.h"
#include <vector>
#include <string>
#include "../graphics/Drawable/BindComponent/bindable_components.h"
#include <memory>

struct Render
{
	MeshPtr pMesh;
	MaterialPtr pMaterial;
	Topology topology;
	bool is_rendered;

};
