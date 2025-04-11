#ifndef __RENDER_H__
#define __RENDER_H__

#include "Mesh.h"
#include "Material.h"
#include "graphics/Drawable/BindComponent/bindable_components.h"

#include <vector>
#include <string>
#include <memory>

SERIALIZE
struct Render
{
	SERIALIZE_FIELD MeshPtr pMesh;
	SERIALIZE_FIELD MaterialPtr pMaterial;
	Topology topology;
};

#endif
