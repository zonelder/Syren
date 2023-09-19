#include "OrientationSystem.h"
#include "../common/TransformUtils.h"


void OrientationSystem::onFrame(SceneManager& scene)
{
	ComponentPool<Transform>& _transforms = scene.getPool<Transform>();
	Transform& mainCamTr = scene.getCamera().transform;
	for (auto& [entID, tr] : _transforms)
	{
		tr.orientationMatrix = DirectX::XMMatrixTranspose(
			toOrientationMatrix(tr) *
			mainCamTr.orientationMatrix

		);
	}

}
