#include "App.h"

#include "systems/orientation_system.h"
#include "systems/render_system.h"
#include "systems/timed_rotationt_system.h"
#include "systems/camera_controller.h"
#include "systems/parent_system.h"
#include "systems/ui_text_render.h"
#include "systems/text_render_system.h"
#include "systems/cell_game.h"
#include "systems/tile_system.h"
#include "systems/fps_counter_system.h"
#include "systems/editor_grid_render.h"

#include "components/timed_rotation.h"
#include "components/parent.h"
#include "components/text_ui.h"
#include "components/text.h"
#include "components/game_cell.h"
#include "components/fps_counter.h"
#include "components/tilemap.h"

#include "geometry_cast.h"
#include  "serialization/scene_loader.hpp"

#include <chrono> 

#include "common/˝ontainers/sparse_page.h"
#include <random>

void App::onInit()
{
	srand((unsigned int)time(0));
	auto gfx = SceneContext::pGfx();
	SceneLoader saver(_scene);

	/// init systems
	//TODO add system serialization
	_systemManager.add<EditorGridRenderer>();
	_systemManager.add<OrientationSystem>();
	_systemManager.add<ParentSystem>();

	_systemManager.add<RenderSystem>();

	_systemManager.add<CellGameSystem>();
	_systemManager.add<TimedRotationSystem>();
	_systemManager.add<CameraController>();
	_systemManager.add<UITextRender>(*gfx, L"resource/myfile.spritefont");
	_systemManager.add<TileSystem>();
	_systemManager.add<FPSCounterSystem>();


	_scene.getCamera().aspectRatio = float(_wnd.GetWidth()) / _wnd.GetHeight();

	_scene.getCamera().transform.position = {0.0f,0.0f,-4.0f};
	//saver.load("resource\\example\\tile_test\\tile_test.xml");

	auto& entt = _scene.createEntity();
	auto& text = _scene.addComponent<TextUI>(entt);
	auto& fpsCounter = _scene.addComponent<FPSCounter>(entt);
	auto& viewMark = _scene.addComponent<FPSView>(entt);

	text.position = { 10.0f,10.0f };

	auto ids = instantiate(_scene,SceneContext::resources().getFbxPrefab("resource/example/fbx_load/SlothSword.fbx"));
	for (auto [_, id] : ids)
	{
		auto& r = _scene.getComponent<Render>(id);
		r.pMaterial->pTexture = context::getTexture("resource/example/fbx_load/textures/Material_BaseColor.png");
		r.pMaterial->color = { 1.0f,1.0f,1.0f,1.0f };
	}
}