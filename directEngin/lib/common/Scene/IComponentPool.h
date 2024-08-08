#pragma once
#include "SceneBase.h"

class IComponentPool
{
public:
	//for delete entity only
	virtual bool removeComponent(EntityID) = 0;
	/*
	virtual void addComponent(EntityID) = 0;
	virtual bool hasComponent(EntityID) const = 0;
	virtual void removeAll() = 0;
	*/
	virtual ~IComponentPool() = default;
protected:
	IComponentPool() = default;
};
