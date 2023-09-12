#include "OrientationSystem.h"
#include "../common/TransformUtils.h"


void OrientationSystem::OnFrame(Transform& objTrnsf,const Transform& cameraTrnsf)
{
	objTrnsf.orientationMatrix = DirectX::XMMatrixTranspose(
		toOrientationMatrix(objTrnsf) *
		cameraTrnsf.orientationMatrix

	);
}
