#pragma once
#include "Camera/Camera.h"
#include "../Window.h"
#include <unordered_map>
#include "../../graphics/Drawable/Primitive.h"
#include "ComponentPool.h"
#include "ComponentManager.h"

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
	void onStartFrame();

	void onEndFrame();
private:
	Graphics _gfx;
	ComponentManager _ComponentManager;
	Camera _mainCamera;

};

