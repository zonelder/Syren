#pragma once

using EntityID = unsigned int;

class IComponentPool
{
public:
	virtual void addComponent(EntityID) = 0;
	virtual bool hasComponent(EntityID) const = 0;
	virtual void removeComponent(EntityID) = 0;
	virtual ~IComponentPool() = default;
};
