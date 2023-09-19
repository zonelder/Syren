#include "SceneManager.h"



SceneManager::SceneManager(const Window& wnd):_gfx(wnd.GetHWND()){
	_mainCamera.aspectRatio = wnd.GetWidth() / wnd.GetHeight();


	_ComponentManager.addPool<Transform>();
	_ComponentManager.addPool<Render>();
 
	EntityID first = 0;
	Transform& t = _ComponentManager.addComponent<Transform>(first);
	Render& r = _ComponentManager.addComponent<Render>(first);
	r.mesh = Primitive::CreateBoxMesh();
	Primitive::InitBinds(_gfx, r, t);
	 
	EntityID second = 1;
	Transform& t2 = _ComponentManager.addComponent<Transform>(second);
	Render& r2 = _ComponentManager.addComponent<Render>(second);
	r2.mesh = Primitive::createCylinderMesh(24);
	Primitive::InitBinds(_gfx, r2, t2);
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
