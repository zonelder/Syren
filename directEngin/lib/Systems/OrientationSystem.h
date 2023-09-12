#pragma once
#include "../component/Transform.h"

class OrientationSystem
{

public:
	void OnFrame(Transform& objTrnsf,const Transform& cameraTrnsf);
};

