#pragma once


using ComponentID = long unsigned int;
const ComponentID MAX_COMPONENT = 64;
using EntityID = unsigned long long int;
const EntityID MAX_ENTITY = 10000u;

constexpr auto g_invalidEntity = EntityID(-1);