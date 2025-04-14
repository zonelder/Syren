#ifndef __BASE_SYSTEM_H__
#define __BASE_SYSTEM_H__

#include "scene_manager.h"

/// @brief base class for Systems
class BaseSystem
{

public:

	virtual void onInit(SceneManager& scene) {};

	virtual void onUpdate(SceneManager& scene, float dt) {};

	virtual void onFrame(SceneManager& scene) {};
};

#endif