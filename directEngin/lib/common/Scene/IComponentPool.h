#pragma once
#include "SceneBase.h"

class IComponentPool
{
public:
	/*
	virtual void addComponent(EntityID) = 0;
	virtual bool hasComponent(EntityID) const = 0;
	virtual bool removeComponent(EntityID) = 0;
	virtual void removeAll() = 0;
	*/
private:
	IComponentPool();
	virtual ~IComponentPool() = default;
};
