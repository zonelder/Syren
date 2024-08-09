#pragma once
#include "App.h"

struct Hit
{
	Hit(Entity& entt) :entt(entt.getID()) {};
	explicit Hit(EntityID id):entt(id)
	{
	}
	const EntityID entt;
};

class GeometryCast
{
public:
	static Hit raycast(SceneManager& scene, DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 dir);
private:

	static DirectX::XMFLOAT3 getGlobalPos(DirectX::XMMATRIX& world, DirectX::XMFLOAT3 pos);

	static bool isInsideBox(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& mibBox, const DirectX::XMFLOAT3& maxBox) noexcept;
	static float getVectroComponent(const DirectX::XMFLOAT3& vector, int index);

	static bool IsBoxHit(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& dir, const DirectX::XMFLOAT3& mibBox, const DirectX::XMFLOAT3& maxBox);

};