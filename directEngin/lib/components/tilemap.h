#pragma once
#include "common/Scene/scene_base.h"

/// @brief storage for Tile-specific data and entity, that represent this tile
struct TileData
{
	EntityID entt;
};

/// @brief Marker class for entity that used by Tilemap
struct Tile
{
	///proposal: may be should add index in tilemap and EntityID of tilemap holder to make linked list form whole structure
};

struct Selected
{

};

/// @brief Represent chunk of tiles in Scene
struct TileMap
{
	static constexpr size_t N = 10;


	TileData tiles[N][N];

};