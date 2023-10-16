#include "SceneManager.h"
#include "../../component/TimedRotation.h"


SceneManager::SceneManager(const Window& wnd):_gfx(wnd.GetHWND()){
	_mainCamera.aspectRatio = wnd.GetWidth() / wnd.GetHeight();


	_ComponentManager.addPool<Transform>();
	_ComponentManager.addPool<Render>();
 
	EntityID first = 0;
	Transform& t = _ComponentManager.addComponent<Transform>(first);
	Render& r = _ComponentManager.addComponent<Render>(first);
	TimedRotation& tr = _ComponentManager.addComponent<TimedRotation>(first);

	tr.xSence = 0.0f;
	tr.ySence = 1.0f;
	tr.zSence = 1.0f;
	r.mesh = Primitive::CreateBoxMesh();
	Primitive::InitBinds(_gfx, r, t);
	 
	EntityID second = 1;
	Transform& t2 = _ComponentManager.addComponent<Transform>(second);
	Render& r2 = _ComponentManager.addComponent<Render>(second);
	TimedRotation& tr2 = _ComponentManager.addComponent<TimedRotation>(second);

	tr2.xSence = 1.0f;
	tr2.ySence = 0.0f;
	tr2.zSence = 1.0f;
	r2.mesh = Primitive::createCylinderMesh(24);
	t2.scale.y = 3.0f;
	Primitive::InitBinds(_gfx, r2, t2);
	for (int i = 0; i < 1000; ++i)
	{
		EntityID id = i + second + 1;
		Transform& t = _ComponentManager.addComponent<Transform>(id);
		Render& r = _ComponentManager.addComponent<Render>(id);
		TimedRotation& tr = _ComponentManager.addComponent<TimedRotation>(id);
		tr.zSence = 1.2f;
		r.mesh = Primitive::Create2SidedPlaneMesh();
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
