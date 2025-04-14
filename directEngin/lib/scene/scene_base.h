#ifndef __SCENE_BASE_H__
#define __SCENE_BASE_H__

using ComponentID = long unsigned int;
const ComponentID MAX_COMPONENT = 64;
using EntityID = unsigned long long int;
const EntityID MAX_ENTITY = 10000u;

constexpr auto INVALID_ENTITY = EntityID(-1);

#endif