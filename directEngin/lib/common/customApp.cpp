#include "App.h"
#include "../Systems/OrientationSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/TimedRotationtSystem.h"
#include "../Systems/CameraController.h"
#include "../Systems/ParentSystem.h"
#include "../Systems/UITextRender.h"
#include "../Systems/TextRenderSystem.h"
#include "../Systems/CellGame.h"

#include "../component/TimedRotation.h"
#include "../component/Parent.h"
#include "../component/TextUI.h"
#include "../component/text.h"
#include "../component/GameCell.h"

#include "GeometryCast.h"



void App::OnInit()
{
	auto& gfx = _scene.getGraphic();
	/// init systems
	_systemManager.add<OrientationSystem>();
	_systemManager.add<ParentSystem>();


	_systemManager.add<RenderSystem>(gfx);
	//_systemManager.add<TextRenderSystem>(gfx);

	_systemManager.add<CellGameSystem>();
	_systemManager.add<TimedRotationSystem>();
	_systemManager.add<CameraController>();
	_systemManager.add<UITextRender>(gfx, L"resource/myfile.spritefont");

	////

	_scene.getCamera().aspectRatio = _wnd.GetWidth() / _wnd.GetHeight();

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

	auto& view = _scene.view<Transform,Render>();
	auto& t   = view.get<Transform>(1);
	auto b = view.begin();
	for (auto [entt, tr, r] : view)
	{

	}

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