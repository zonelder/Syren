#pragma once
#include "../common/Scene/BaseSystem.h"
#include "../component/Parent.h"
#include "../component/Transform.h"


class ParentSystem: public BaseSystem
{
public:
	void onFrame(SceneManager&) override;
	void onUpdate(SceneManager& scene,float dt) override;

private:
	void setlGlobalMatrix(ComponentPool<Transform>& trPool, ComponentPool<Parent>& parentPool,EntityID curEntity);
};

