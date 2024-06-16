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

	const auto& proj = scene.getCamera().projection;
	float px = 2.0f*scene.getInput().normedX -1.0f;
	px /= DirectX::XMVectorGetX(proj.r[0]);
	auto py = -2.0f*scene.getInput().normedY + 1.0f;
	py/= DirectX::XMVectorGetY(proj.r[1]);
	auto pz = DirectX::XMVectorGetZ(proj.r[2]);
	const auto& view = scene.getCamera().transform.orientationMatrix;
	DirectX::XMFLOAT3 ray = newBazis({ px,py,1.0f }, view);


	
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

DirectX::XMFLOAT3 CellGameSystem::newBazis(const DirectX::XMFLOAT3& v, const DirectX::XMMATRIX& mat)
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
	DirectX::XMFLOAT3 out;


	out.x = v.x * rX + v.y * rY + v.z * rZ;
	out.y = v.x * uX + v.y * uY + v.z * uZ;
	out.z = v.x * fX + v.y * fY + v.z * fZ;

	return out;
}
