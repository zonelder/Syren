#pragma once
#include "Mesh.h"
#include "Material.h"
#include <vector>
#include <string>
#include "../graphics/Drawable/BindComponent/bindable_components.h"
#include <memory>

SERIALIZE
struct Render
{
	SERIALIZE_FIELD MeshPtr pMesh;
	SERIALIZE_FIELD MaterialPtr pMaterial;
	Topology topology;
	bool is_rendered;

};
