#include "CellGame.h"
#include "../component/GameCell.h"
#include "../common/GeometryCast.h"

void CellGameSystem::onInit(SceneManager& scene)
{

	auto& gfx = scene.getGraphic();
	MaterialPtr select = scene.makeMaterial();
	select->texture.set(gfx, nullptr);
	select->color = { 1.0f,.0f,.0f,1.0f };

	MaterialPtr deselect = scene.makeMaterial();
	deselect->texture.set(gfx, nullptr);
	deselect->color = { 0.0f,.0f,.0f,1.0f };

	p_selectMat = select;
	p_deselectMat = deselect;

}

void CellGameSystem::onUpdate(SceneManager& scene, float time)
{
	DirectX::XMFLOAT3 ray = { 0.0f,0.0f,1.0f };
	auto& cameraPos = scene.getCamera().transform.position;
	auto hit = GeometryCast::raycast(scene, cameraPos, ray);
	EntityID hitID = -1;
	if (hit.entt.getID() != -1 && scene.hasComponent<GameCell>(hit.entt))
	{
		hitID = hit.entt.getID();
	}
	for (auto& enttID : scene.getEntitiesWith<GameCell, Render,Transform>())
	{

		auto& cell = scene.getComponent<GameCell>(enttID);
		auto& render = scene.getComponent<Render>(enttID);
		cell.isSelected = enttID.getID() == hitID;
		render.p_material = (cell.isSelected) ? (p_selectMat) : (p_deselectMat);

	}
}
