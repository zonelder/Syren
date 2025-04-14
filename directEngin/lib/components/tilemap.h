#pragma once
#include "scene/scene_base.h"

/// @brief storage for Tile-specific data and entity, that represent this tile
SERIALIZE
struct TileData
{
	SERIALIZE_FIELD EntityID entt;
};

/// @brief Marker class for entity that used by Tilemap
SERIALIZE
struct Tile
{
	///proposal: may be should add index in tilemap and EntityID of tilemap holder to make linked list form whole structure
};

SERIALIZE
struct Selected
{

};


/// @brief Represent chunk of tiles in Scene
SERIALIZE
struct TileMap
{
	static constexpr size_t N = 10;


	SERIALIZE_FIELD TileData tiles[N*N];

	TileData& operator()(size_t x, size_t y)
	{
		return tiles[x + N * y];
	}
};