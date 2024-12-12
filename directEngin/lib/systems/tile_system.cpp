#include "tile_system.h"
#include "components/tilemap.h"

void TileSystem::onInit(SceneManager& scene)
{
	auto& resMngr = scene.resManager();

	p_selectMat = resMngr.getMaterial(".\\resource\\example\\material\\tile_red.syrenmaterial");
	p_deselectMat = resMngr.getMaterial(".\\resource\\example\\material\\tile_black.syrenmaterial");
}

#pragma optimize("", off)
void TileSystem::onUpdate(SceneManager& scene,float time)
{

	static float last_time = 0.0f;
	//last_time += time/100;
	if (time - last_time < 1.0f)
	{
		return;
	}
	last_time = time;
	auto& view = scene.view<Selected, Tile, Render>();
	for (auto [chunkEntt, chunk] : scene.view<TileMap>())
	{
		for (size_t x = 0;x < TileMap::N;++x)
		{
			for (size_t y = 0; y < TileMap::N; ++y)
			{
				const auto& data = chunk.tiles[x][y];
				if (!view.contains(data.entt))
					continue;

				scene.removeComponent<Selected>(data.entt);
				auto& r = view.get<Render>(data.entt);
				r.pMaterial = p_deselectMat;

				int x_random = 1 - 2*(rand() % 2);
				if (x_random + x >= 10)
					x_random = 0;
				if (x_random < 0 && x == 0)
					x_random = 0;

				int y_random = 1 - 2*(rand() % 2);

				if (y_random + y >= 10)
					y_random = 0;
				else if (y_random < 0 && y == 0)
					y_random = 0;
				if (x_random == y_random && x_random == 0)
				{
					r.pMaterial = p_selectMat;
					return;
				}
				const auto& newSelectedData = chunk.tiles[x_random+x][y_random+y];
				scene.addComponent<Selected>(newSelectedData.entt);
				r = view.get<Render>(newSelectedData.entt);
				r.pMaterial = p_selectMat;
				return;

			}
		}
	}
}
