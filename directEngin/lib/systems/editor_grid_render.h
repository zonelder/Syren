#ifndef  __EDITOR_GRID_RENDERER__
#define __EDITOR_GRID_RENDERER__


#include "scene/base_system.h"
#include "resmgr/material.h"
#include <vector>

class EditorGridRenderer : public BaseSystem
{
public:
	void onInit(SceneManager& scene) override;
	void onUpdate(SceneManager& scene, float time) override;

private:
	EntityID  _enttID;
};

#endif // ! __EDITOR_GREED_RENDERER__
