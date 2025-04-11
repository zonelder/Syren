#ifndef __GEOMETRY_CAST_H__
#define __GEOMETRY_CAST_H__
#include "App.h"
#include "math/vector3.h"

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
	static Hit raycast(SceneManager& scene, const Vector3& origin, const Vector3&  dir);
private:
	static bool isInsideBox(const Vector3& position, const Vector3& mibBox, const Vector3& maxBox) noexcept;

	static bool IsBoxHit(const Vector3& position, const Vector3& dir, const Vector3& mibBox, const Vector3& maxBox);

};

#endif