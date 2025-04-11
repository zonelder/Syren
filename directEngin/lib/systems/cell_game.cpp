#include "cell_game.h"
#include "components/game_cell.h"
#include "common/geometry_cast.h"

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
	/*
	const auto& camera = scene.getCamera();
	const auto& proj = camera.projection();
	float px = 2.0f*context::input().normedX - 1.0f;
	px /= DirectX::XMVectorGetX(proj.r[0]);
	auto py = -2.0f* context::input().normedY + 1.0f;
	py/= DirectX::XMVectorGetY(proj.r[1]);
	auto pz = DirectX::XMVectorGetZ(proj.r[2]);
	const auto& view = camera.transform.orientationMatrix;
	auto ray = newBazis({ px,py,1.0f }, view);


	
	auto& cameraPos = camera.transform.position;
	auto hit = GeometryCast::raycast(scene, cameraPos, ray);

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
	*/
}

Vector3  CellGameSystem::newBazis(const Vector3& v, const DirectX::XMMATRIX& mat)
{
	auto right = DirectX::XMVector3Normalize(mat.r[0]);
	
	float rX = DirectX::XMVectorGetX(right);
	float rY = DirectX::XMVectorGetY(right);
	float rZ = DirectX::XMVectorGetZ(right);
	auto up = DirectX::XMVector3Normalize(mat.r[1]);
	float uX = DirectX::XMVectorGetX(up);
	float uY = DirectX::XMVectorGetY(up);
	float uZ = DirectX::XMVectorGetZ(up);
	auto forward = DirectX::XMVector3Normalize(mat.r[2]);
	float fX = DirectX::XMVectorGetX(forward);
	float fY = DirectX::XMVectorGetY(forward);
	float fZ = DirectX::XMVectorGetZ(forward);
	Vector3 out;


	out[0] = v[0] * rX + v[1] * rY + v[2] * rZ;
	out[1] = v[0] * uX + v[1] * uY + v[2] * uZ;
	out[2] = v[0] * fX + v[1] * fY + v[2] * fZ;

	return out;
}
