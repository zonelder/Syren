#include "App.h"
#include "../Systems/OrientationSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/TimedRotationtSystem.h"
#include "../Systems/CameraController.h"
#include "../Systems/ParentSystem.h"
#include "../Systems/UITextRender.h"

#include "../component/TimedRotation.h"
#include "../component/Parent.h"
#include "../component/TextUI.h"



void App::OnInit()
{
	auto& gfx = _sceneManager.getGraphic();
	/// init systems
	_systemManager.add<OrientationSystem>();
	_systemManager.add<ParentSystem>();
	_systemManager.add<RenderSystem>();

	_systemManager.add<TimedRotationSystem>();
	_systemManager.add<CameraController>();
	_systemManager.add<UITextRender>(gfx, L"myfile.spritefont");

	////

	_sceneManager.getCamera().aspectRatio = _wnd.GetWidth() / _wnd.GetHeight();



	auto material = _sceneManager.makeMaterial();
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

	EntityID second = 1;
	auto p_plane_mesh = _sceneManager.make2SidedPlaneMesh();
	EntityID id = second;
	Transform& t = _sceneManager.addComponent<Transform>(id);
	Render& r = _sceneManager.addComponent<Render>(id);
	TimedRotation& tr = _sceneManager.addComponent<TimedRotation>(id);
	tr.zSence = 1.0f;
	tr.xSence = 0.0f;
	r.p_mesh = p_plane_mesh;
	r.p_material = material;
	t.position = { 1.0f,0.0f,1.0f };
	const int num = 1;
	auto card_mat = _sceneManager.makeMaterial();
	card_mat->texture.set(gfx, L"card_test.dds");
	for (int i = 0; i < num; ++i)
	{
		EntityID id = i + second + 1;
		Transform& t = _sceneManager.addComponent<Transform>(id);
		Render& r = _sceneManager.addComponent<Render>(id);
		TimedRotation& tr = _sceneManager.addComponent<TimedRotation>(id);
		tr.zSence = 1.2f;
		t.scale = { 0.5f,0.5f,0.5f };

		//tr.xSence = 1.0f;
		r.p_mesh = p_plane_mesh;
		r.p_material = card_mat;
		/*
		if (i == num)
		{
			auto& p = addComponent<Parent>(second);
			p.parent = id;
			t.position = { 0.0f,0.0f,0.0f };
			tr.zSence = 1.0f;
			continue;
		}
				*/
		Parent& p = _sceneManager.addComponent<Parent>(id);
		p.parent = second;


		//t.position = { float(i % 10)*3.0f,0.0f,float(i / 10)*3.0f };
		t.position = { 0.0f,0.0f,-0.0001f };
	}

	EntityID text_id = num + 4;
	TextUI& text1 = _sceneManager.addComponent<TextUI>(text_id);
	text1.content = L"hello world";
	text1.position = DirectX::XMFLOAT2(10, 10);

	TextUI& text2 = _sceneManager.addComponent<TextUI>(text_id + 1);
	text2.content = L"Test";
	text2.position = DirectX::XMFLOAT2(10, 20);


}