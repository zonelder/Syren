#pragma once
#include "Camera/Camera.h"
#include "../Window.h"
#include <unordered_map>
#include "../../Systems/RenderSystem.h"
#include "../../Systems/OrientationSystem.h"
#include "../../graphics/Drawable/Primitive.h"
#include "ComponentPool.h"

using EntityID = unsigned int;

class SceneManager
{
public:
	SceneManager(const Window& wnd);


	void Update(float dt);
	void Frame();

private:
	Graphics _gfx;
	ComponentPool<Transform> _transforms;
	ComponentPool<Render> _renders;
	RenderSystem _renderSystem;
	OrientationSystem _orientationSystem;
	Camera _mainCamera;

};

