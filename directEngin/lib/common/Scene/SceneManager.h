#pragma once
#include "Camera/Camera.h"
#include "../Window.h"
#include <unordered_map>
#include "../../graphics/Drawable/Primitive.h"
#include "ComponentPool.h"
#include "ComponentManager.h"
#include "../../component/Render.h"
#include "../../component/Transform.h"

using EntityID = unsigned int;

class SceneManager
{
public:
	SceneManager(const Window& wnd);


	Camera& getCamera() noexcept;

	Graphics& getGraphic() noexcept;


	template<typename T>
	ComponentPool<T>& getPool()
	{
		return *(_ComponentManager.getPool<T>());
	}

	template<typename T>
	T& addComponent(EntityID id)
	{
		return _ComponentManager.addComponent<T>(id);
	}

	void onStartFrame();

	void onEndFrame();
private:
	Graphics _gfx;
	ComponentManager _ComponentManager;
	Camera _mainCamera;

};

