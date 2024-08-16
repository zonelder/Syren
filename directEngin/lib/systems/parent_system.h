#pragma once
#include "../common/Scene/BaseSystem.h"
#include "components/parent.h"
#include "components/transform.h"


class ParentSystem: public BaseSystem
{
public:
	void onFrame(SceneManager&) override;
	void onUpdate(SceneManager& scene,float dt) override;

private:
	void setlGlobalMatrix(ComponentPool<Transform>& trPool, ComponentPool<Parent>& parentPool,EntityID curEntity);
};

