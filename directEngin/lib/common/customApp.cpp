#include "App.h"
#include "../Systems/OrientationSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/TimedRotationtSystem.h"
#include "../Systems/CameraController.h"
#include "../Systems/ParentSystem.h"
#include "../Systems/UITextRender.h"
#include "../Systems/TextRenderSystem.h"

#include "../component/TimedRotation.h"
#include "../component/Parent.h"
#include "../component/TextUI.h"
#include "../component/text.h"



void App::OnInit()
{
	auto& gfx = _scene.getGraphic();
	/// init systems
	_systemManager.add<OrientationSystem>();
	_systemManager.add<ParentSystem>();


	_systemManager.add<RenderSystem>();
	//_systemManager.add<TextRenderSystem>(gfx);

	//_systemManager.add<TimedRotationSystem>();
	_systemManager.add<CameraController>();
	_systemManager.add<UITextRender>(gfx, L"myfile.spritefont");

	////

	_scene.getCamera().aspectRatio = _wnd.GetWidth() / _wnd.GetHeight();

	_scene.getCamera().transform.position = {0.0f,0.0f,-4.0f};

	auto material = _scene.makeMaterial();
	material->texture.set(gfx, L"test_texture.dds");
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
	*/

	const auto& entt1 = _scene.createEntity();
	auto p_plane_mesh = _scene.make2SidedPlaneMesh();
	Transform& t = _scene.addComponent<Transform>(entt1);
	Render& r = _scene.addComponent<Render>(entt1);
	TimedRotation& tr = _scene.addComponent<TimedRotation>(entt1);
	tr.zSence = 1.0f;
	tr.xSence = 0.0f;
	//Text& text = _scene.addComponent<Text>(id);
	//text.content = L"Test Content";
	r.p_mesh = p_plane_mesh;
	r.p_material = material;
	t.position = { 0.0f,0.0f,0.0f};

	const auto& plane_view = _scene.createEntity();

	auto& plane_cords = _scene.addComponent<TextUI>(plane_view);
	plane_cords.position = { 10,10 };
	plane_cords.content = L"text view";

}