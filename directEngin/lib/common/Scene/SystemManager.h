#pragma once
#include <list>
#include <vector>
#include "BaseSystem.h"
#include <memory>
#include <algorithm>

template<typename T,typename U>
concept DeriveFrom = std::is_base_of<U, T>::value;

class SystemManager
{
public:
	template<typename T>
	void addSystem() requires DeriveFrom<T, BaseSystem>
	{
		_systems.emplace_back(new T);
	}

	void update(SceneManager&,float dt);

	void frame(SceneManager&);

private:
	//std::vector<int> _priorities;
	std::vector<std::unique_ptr<BaseSystem>>  _systems;
};

