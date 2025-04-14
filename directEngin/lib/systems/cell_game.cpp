#include "cell_game.h"
#include "components/game_cell.h"
#include "components/render.h"

#include "common/geometry_cast.h"
#include "resmgr/scene_context.h"
#include "resmgr/mesh.h"
#include "math/vector2.h"

void CellGameSystem::onInit(SceneManager& scene)
{

	p_selectMat = context::getMaterial("resource\\example\\tile_test\\material\\tile_red.syrenmaterial");
	p_deselectMat = context::getMaterial("resource\\example\\tile_test\\material\\tile_black.syrenmaterial");

	for (auto [enttID,cell,render] : scene.view<GameCell, Render>())
	{
		render.pMaterial = p_deselectMat;
	}

}

void CellGameSystem::onUpdate(SceneManager& scene, float time)
{
	const auto& camera = scene.getCamera();
	const auto& proj = camera.projection();
	Vector2 clipPos(2.0f * context::input().normedX - 1.0f, -2.0f * context::input().normedY + 1.0f);
	auto toWorld = camera.view()*camera.projection();
	toWorld = toWorld.inverse();
	auto rayStart = toWorld.multiplyPoint({ clipPos[0],clipPos[1],0.0f });
	auto rayEnd = toWorld.multiplyPoint({ clipPos[0],clipPos[1],1.0f });
//TODO check working
	auto& cameraPos = camera.transform.position;
	auto hit = GeometryCast::raycast(scene, cameraPos, (rayEnd - rayStart).normalized());

	if (hit.entt != -1 && scene.hasComponent<GameCell>(hit.entt))
	{
		scene.addComponent<Chained>(hit.entt);
		auto& render = scene.getComponent<Render>(hit.entt);
		render.pMaterial = p_selectMat;
	}
	auto& viewComp = scene.view<Chained, GameCell, Render>();
	for (auto [enttID,ch,cell,render] : viewComp)
	{

		if (hit.entt != enttID)
		{
			//scene.removeComponent<Chained>(enttID);
			render.pMaterial = p_deselectMat;
		}
	}
}
