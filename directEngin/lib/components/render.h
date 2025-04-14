#ifndef __RENDER_H__
#define __RENDER_H__

#include "resmgr/mesh.h"
#include "resmgr/material.h"

#include "graphic/bindable_components.h"
#include "serialization/base.h"

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
