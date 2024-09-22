#include "geometry_cast.h"
#include "components/transform.h"
#include <limits>
#include <math.h>

Hit GeometryCast::raycast(SceneManager& scene, DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 dir)
{


	for (auto [entt,render,tr] : scene.view<Render,Transform>())
	{
		const auto& bb = scene.getMeshData(render.p_mesh)->boundingBox;

		const auto minGlobal = getGlobalPos(tr.orientationMatrix, bb.minBound);
		const auto maxGlobal = getGlobalPos(tr.orientationMatrix, bb.maxBound);

		if (IsBoxHit(origin, dir, minGlobal, maxGlobal))
		{
			return Hit(entt);
		}

	}
	return Hit(-1);
}

DirectX::XMFLOAT3 GeometryCast::getGlobalPos(DirectX::XMMATRIX& world, DirectX::XMFLOAT3 pos)
{
	const auto matrix = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotationQuat;
	DirectX::XMVECTOR translation;
	DirectX::XMMatrixDecompose(&scale, &rotationQuat, &translation, matrix);

	// Вектор трансляции
	DirectX::XMFLOAT3 global;
	DirectX::XMStoreFloat3(&global, translation);
	return global;
}

bool GeometryCast::isInsideBox(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max) noexcept
{
	return (position.x >= min.x && position.x <= max.x) &&
		(position.y >= min.y && position.y <= max.y) &&
		(position.z >= min.z && position.z <= max.z);
}

float GeometryCast::getVectroComponent(const DirectX::XMFLOAT3& vector, int index)
{
	if (index == 0)
		return vector.x;
	if (index == 1)
		return vector.y;
	if (index == 2)
		return vector.z;

	return 0.0f;
}

bool GeometryCast::IsBoxHit(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& dir, const DirectX::XMFLOAT3& minGlobal, const DirectX::XMFLOAT3& maxGlobal)
{
	if (isInsideBox(origin, minGlobal, maxGlobal))
	{
		return true;
	}

	float t1, t2;

	float t_near = (std::numeric_limits<float>::min)();
	float t_far = (std::numeric_limits<float>::max)();

	for (int i = 0; i < 3; ++i)
	{
		float originC = getVectroComponent(origin, i);
		float dirC = getVectroComponent(dir, i);
		float minC = getVectroComponent(minGlobal, i);
		float maxC = getVectroComponent(maxGlobal, i);
		if (std::abs(dirC) < std::numeric_limits<float>::epsilon())
		{
			if (originC < minC || originC > maxC)
				return false;
		}
		else
		{
			t1 = (minC - originC) / dirC;
			t2 = (maxC - originC) / dirC;

			if (t1 > t2)
				std::swap(t1, t2);
			if (t1 > t_near)
				t_near = t1;
			if (t2 < t_far)
				t_far = t2;

			if (t_near > t_far)
				return false;
			if (t_far < 0.0f)
				return false;
		}
	};

	return (t_near <= t_far && t_far >= 0.0f);
}
