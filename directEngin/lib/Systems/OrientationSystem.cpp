#include "OrientationSystem.h"
#include "../common/TransformUtils.h"
#include "../component/TextUI.h"
#include <iostream>


void OrientationSystem::onFrame(SceneManager& scene)
{
	ComponentPool<Transform>& _transforms = scene.getPool<Transform>();
	//auto& output = scene.getPool<TextUI>().getComponent(1);
	//auto& cameraOr = scene.getCamera().transform.orientationMatrix;
	for (auto& [entID, tr] : _transforms)
	{
		tr.orientationMatrix = toOrientationMatrix(tr);
		/*
		output.content =
			std::to_wstring(tr.orientationMatrix.r[0].m128_f32[3]) + L" " +
			std::to_wstring(tr.orientationMatrix.r[1].m128_i64[3]) + L" " +
			std::to_wstring(tr.orientationMatrix.r[2].m128_i64[3]) + L" " +
			std::to_wstring(tr.orientationMatrix.r[3].m128_i64[3]);
			*/
	}

}
