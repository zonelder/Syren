#pragma once
#include "Camera/Camera.h"
#include "../Window.h"
#include <unordered_map>
#include "../../Systems/RenderSystem.h"
#include "../../Systems/OrientationSystem.h"
#include "../../graphics/Drawable/Primitive.h"

using EntityID = unsigned int;

class SceneManager
{
public:
	SceneManager(const Window& wnd);


	void Update(float dt);
	void Frame();

private:
	Graphics _gfx;
	std::unordered_map<EntityID, Transform> _transforms;
	std::unordered_map<EntityID, Render> _renders;
	RenderSystem _renderSystem;
	OrientationSystem _orientationSystem;
	Camera _mainCamera;

};

