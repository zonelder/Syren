#include "SceneManager.h"
#include "../../component/TimedRotation.h"


SceneManager::SceneManager(const Window& wnd):_gfx(wnd.GetHWND()){
	_mainCamera.aspectRatio = wnd.GetWidth() / wnd.GetHeight();


	_ComponentManager.addPool<Transform>();
	_ComponentManager.addPool<Render>();
	

	auto material = std::make_shared<Material>();
	material->vertexShader = L"VertexShader.cso";
	material->pixelShader = L"PixelShader.cso";
	material->init(_gfx);

	EntityID first = 0;
	Transform& t = addComponent<Transform>(first);
	Render& r = addComponent<Render>(first);
	TimedRotation& tr = addComponent<TimedRotation>(first);

	tr.xSence = 0.0f;
	tr.ySence = 1.0f;
	tr.zSence = 1.0f;
	r.p_mesh = Primitive::CreateBoxMesh();
	r.p_mesh->init(_gfx);
	r.p_material = material;
	Primitive::InitBinds(_gfx, r, t);
	 
	EntityID second = 1;
	Transform& t2 = addComponent<Transform>(second);
	Render& r2 = addComponent<Render>(second);
	TimedRotation& tr2 = addComponent<TimedRotation>(second);

	tr2.xSence = 1.0f;
	tr2.ySence = 0.0f;
	tr2.zSence = 1.0f;
	r2.p_mesh = Primitive::createCylinderMesh(24);
	r2.p_mesh->init(_gfx);
	r2.p_material = material;
	t2.scale.y = 3.0f;
	Primitive::InitBinds(_gfx, r2, t2);

	auto p_plane_mesh = Primitive::Create2SidedPlaneMesh();
	p_plane_mesh->init(_gfx);
	for (int i = 0; i < 10000; ++i)
	{
		EntityID id = i + second + 1;
		Transform& t = addComponent<Transform>(id);
		Render& r = addComponent<Render>(id);
		TimedRotation& tr = addComponent<TimedRotation>(id);
		tr.zSence = 1.2f;
		r.p_mesh = p_plane_mesh;
		r.p_material = material;
		Primitive::InitBinds(_gfx, r, t);// this is a problem that user need to to "bind" by hands
		t.position = { float(i % 10)*3.0f,0.0f,float(i / 10)*3.0f };
	}
}



void SceneManager::onStartFrame()
{
	_mainCamera.OnFrame();
	_gfx.ClearBuffer(_mainCamera.background);
}

void SceneManager::onEndFrame()
{
	_gfx.endFrame();
}


Camera& SceneManager::getCamera() noexcept
{
	return _mainCamera;
}

Graphics& SceneManager::getGraphic() noexcept
{
	return _gfx;
}
