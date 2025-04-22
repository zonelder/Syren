#ifndef __SCENE_BASE_H__
#define __SCENE_BASE_H__

#include <limits>

using ComponentID = long unsigned int;
const ComponentID MAX_COMPONENT = 64;
using EntityID = unsigned long long int;
const EntityID MAX_ENTITY = 10000u;

constexpr auto tombstone = std::numeric_limits<EntityID>::max();

#endif