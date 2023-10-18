#pragma once
#include "IDrawable.h"
#include <vector>
#include <DirectXMath.h>
#include "BindComponent/IBindable.h"
#include "../../component/Transform.h"
#include "../../component/Render.h"
#include <memory>

class Primitive
{
public:
	Primitive();
	Primitive(Mesh&);

	/// @brief reset all binds and build new 
	static void InitBinds(Graphics&,Render&,Transform&);

	using shared_mesh = std::shared_ptr<Mesh>;
	static shared_mesh CreateBoxMesh();
	static shared_mesh createCylinderMesh(unsigned int n = 8);
	static shared_mesh Create2SidedPlaneMesh();

};

