#include "geometry_cast.h"
#include "components/transform.h"
#include <limits>

Hit GeometryCast::raycast(SceneManager& scene, const Vector3& origin, const Vector3& dir)
{


	for (auto [entt,render,tr] : scene.view<Render,Transform>())
	{
		if (!render.pMesh)
			continue;
		const auto& bb = render.pMesh->boundingBox;

		const auto minGlobal = tr.orientationMatrix.multiplyPoint(bb.minBound);
		const auto maxGlobal = tr.orientationMatrix.multiplyPoint(bb.maxBound);

		if (IsBoxHit(origin, dir, minGlobal, maxGlobal))
		{
			return Hit(entt);
		}

	}
	return Hit(-1);
}


bool GeometryCast::isInsideBox(const Vector3& position, const Vector3& min, const Vector3& max) noexcept
{
	return (position[0] >= min[0] && position[0] <= max[0]) &&
		(position[1] >= min[1] && position[1] <= max[1]) &&
		(position[2] >= min[2] && position[2] <= max[2]);
}


bool GeometryCast::IsBoxHit(const Vector3& origin, const Vector3& dir, const Vector3& minGlobal, const Vector3& maxGlobal)
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
		float originC = origin[i];
		float dirC = dir[i];
		float minC = minGlobal[i];
		float maxC = maxGlobal[i];
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
