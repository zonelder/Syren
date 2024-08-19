#include "App.h"

#include "systems/orientation_system.h"
#include "systems/render_system.h"
#include "systems/timed_rotationt_system.h"
#include "systems/camera_controller.h"
#include "systems/parent_system.h"
#include "systems/ui_text_render.h"
#include "systems/text_render_system.h"
#include "systems/cell_game.h"

#include "components/timed_rotation.h"
#include "components/parent.h"
#include "components/text_ui.h"
#include "components/text.h"
#include "components/game_cell.h"

#include "geometry_cast.h"


#include <chrono>  //���������� chrono ��� ������ �� ��������


void App::OnInit()
{
	auto& gfx = _scene.getGraphic();
	/// init systems
	_systemManager.add<OrientationSystem>();
	//_systemManager.add<ParentSystem>();


	_systemManager.add<RenderSystem>(_scene);
	//_systemManager.add<TextRenderSystem>(gfx);

	_systemManager.add<CellGameSystem>();
	_systemManager.add<TimedRotationSystem>();
	_systemManager.add<CameraController>();
	_systemManager.add<UITextRender>(gfx, L"resource/myfile.spritefont");

	////

	_scene.getCamera().aspectRatio = float(_wnd.GetWidth()) / _wnd.GetHeight();

	_scene.getCamera().transform.position = {0.0f,0.0f,-4.0f};

	auto p_plane_mesh = _scene.make2SidedPlaneMesh();
	const int cellInLine = 3;
	for (int i = 0; i < cellInLine *2; i+=2)
	{
		for (int j = 0; j < cellInLine*2; j+=2)
		{
			const auto& entt = _scene.createEntity();
			Transform& t = _scene.addComponent<Transform>(entt);
			Render& r = _scene.addComponent<Render>(entt);
			GameCell& cell = _scene.addComponent<GameCell>(entt);
			r.p_mesh = p_plane_mesh;
			t.position = { (float)i,(float)j,0.0f };

		}
	}
	for (auto i = 0; i < MAX_ENTITY - 200; ++i)
	{
		const auto& entt = _scene.createEntity();
		Transform& t = _scene.addComponent<Transform>(entt);
		GameCell& cell = _scene.addComponent<GameCell>(entt);
	}

	const auto test1_start = std::chrono::high_resolution_clock::now();

	const auto test1_diff = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - test1_start).count();


	auto& view = _scene.view<Transform, GameCell>();
	
	const auto test3_start = std::chrono::high_resolution_clock::now();
	for (auto [entt, tr, cell] : view)
	{
		cell.isSelected = std::rand() % 2;
	}
	const auto test3_diff = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - test3_start).count();
	std::cout << "test 1 = " << test1_diff << "\ntest 3 = " << test3_diff << std::endl;

	//*/
	/*
	auto material = _scene.makeMaterial();
	material->texture.set(gfx, L"resource/test_texture.dds");

	material->texture.set(gfx, nullptr);
	/*
	EntityID first = 0;
	Transform& t = addComponent<Transform>(first);
	Render& r = addComponent<Render>(first);
	TimedRotation& tr = addComponent<TimedRotation>(first);

	tr.xSence = 0.0f;
	tr.ySence = 1.0f;
	tr.zSence = 1.0f;
	r.p_mesh = makeBoxMesh();
	r.p_material = material;

	EntityID second = 1;
	Transform& t2 = addComponent<Transform>(second);sys
	Render& r2 = addComponent<Render>(second);
	TimedRotation& tr2 = addComponent<TimedRotation>(second);

	tr2.xSence = 1.0f;
	tr2.ySence = 0.0f;
	tr2.zSence = 1.0f;
	r2.p_mesh = makeCylinderMesh();
	r2.p_material = material;
	t2.scale.y = 3.0f;
	* /

	const auto& entt1 = _scene.createEntity();
	auto p_plane_mesh = _scene.make2SidedPlaneMesh();
	Transform& t = _scene.addComponent<Transform>(entt1);
	Render& r = _scene.addComponent<Render>(entt1);
	_scene.addComponent<GameCell>(entt1);
	//TimedRotation& tr = _scene.addComponent<TimedRotation>(entt1);
	//tr.zSence = 1.0f;
	//tr.xSence = 0.0f;
	//Text& text = _scene.addComponent<Text>(id);
	//text.content = L"Test Content";
	r.p_mesh = p_plane_mesh;
	r.p_material = material;
	t.position = { 0.0f,0.0f,0.0f};

	const auto& entt2 = _scene.createEntity();

	Transform& t2 = _scene.addComponent<Transform>(entt2);
	Render& r2 = _scene.addComponent<Render>(entt2);
	TimedRotation& tr2 = _scene.addComponent<TimedRotation>(entt2);
	tr2.zSence = -2.0f;
	tr2.xSence = 0.0f;
	//Text& text = _scene.addComponent<Text>(id);
	//text.content = L"Test Content";
	r2.p_mesh = p_plane_mesh;
	r2.p_material = material;
	t2.position = { 1.0f,0.0f,0.0f };

	const auto& entt3 = _scene.createEntity();

	Transform& t3 = _scene.addComponent<Transform>(entt3);
	Render& r3 = _scene.addComponent<Render>(entt3);
	TimedRotation& tr3 = _scene.addComponent<TimedRotation>(entt3);
	tr3.zSence = -2.0f;
	tr3.xSence = 0.0f;
	//Text& text = _scene.addComponent<Text>(id);
	//text.content = L"Test Content";
	r3.p_mesh = p_plane_mesh;
	r3.p_material = material;
	t3.position = { 1.0f,2.0f,0.0f };

	const auto& plane_view = _scene.createEntity();

	auto& plane_cords = _scene.addComponent<TextUI>(plane_view);
	plane_cords.position = { 10,10 };
	plane_cords.content = L"text view";
	*/

}