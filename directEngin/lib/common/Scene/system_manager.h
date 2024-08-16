#pragma once
#include <list>
#include <vector>
#include "base_system.h"
#include <memory>
#include <algorithm>
#include <map>

template<typename T,typename U>
concept DeriveFrom = std::is_base_of<U, T>::value;

class SystemManager
{
public:
	SystemManager() :_maxPriority(0){

	};
	/// @brief create new system at the end of call queue
	/// @tparam T Class,that derived from BaseSystem Class
	template<typename T,class... Args>
	void add(Args&&... args) requires DeriveFrom<T, BaseSystem>
	{
		add<T>(_maxPriority + 1,std::forward<Args>(args)...);
	}

	/// @brief create new system in processor with setted priory
	/// @tparam T Class,that derived from BaseSystem Class
	/// @param priority Prioriy of calls. Systems with less priority will be called earlier
	template<typename T, class... Args>
	void add(unsigned int priority,Args&&... args) requires DeriveFrom<T, BaseSystem>
	{
		if (_systems.contains(priority))
		{
			//TODO add warn: attempt to add system with priority that already exist. you should set unique priority to system
			return;
		}
		if (priority > _maxPriority)
		{
			_maxPriority = priority;
		}
		_systems.emplace(priority,new T(std::forward<Args>(args)...));
	}
	

	/// @brief Remove system from the list with specific priority level
	/// @param priority 
	void removeSystem(unsigned int priority) noexcept;

	/// @brief find system with priority level equal curPriority and set new Priority level
	/// @param curPriority current priority level
	/// @param prioruty new priority level
	void resetPriority(unsigned int curPriority, unsigned int priority) noexcept;


	void init(SceneManager& scene);
	/// @brief update all systems
	/// @param  scene Scene, where systems update needed
	/// @param dt time from last update call
	void update(SceneManager& scene,float dt);

	/// @brief call "frame" method of system from added list
	/// @param  scene Scene, where systems update needed
	void frame(SceneManager& scene);

private:
	//std::vector<int> _priorities;
	unsigned int _maxPriority = 0;
	std::map<unsigned int ,std::unique_ptr<BaseSystem>>  _systems;
};

