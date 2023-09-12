#pragma once
#include "IDrawable.h"
#include <vector>
#include <DirectXMath.h>
#include "BindComponent/IBindable.h"
#include "../../component/Transform.h"
#include "../../component/Render.h"


class Primitive
{
public:
	Primitive();
	Primitive(Mesh&);

	/// @brief reset all binds and build new 
	static void InitBinds(Graphics&,Render&,Transform&);


	static Primitive CreateBox();
	static Primitive createCylinder(unsigned int n = 8);

	/// @brief Primitive object mesh data. DO NOT CHANGE THIS DATA STRUCTURE
	Transform transform;
	Render render;
};

