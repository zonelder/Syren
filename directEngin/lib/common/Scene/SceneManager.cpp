#include "SceneManager.h"
#include "../Input.h"



SceneManager::SceneManager(const Window& wnd):_gfx(wnd.GetHWND()){
	_mainCamera.aspectRatio = wnd.GetWidth() / wnd.GetHeight();

	EntityID first = 0;
	Transform& t =_transforms.addComponent(first);
	Render& r = _renders.addComponent(first);
	r.mesh = Primitive::CreateBoxMesh();
	Primitive::InitBinds(_gfx, r, t);

	EntityID second = 1;
	Transform& t2 = _transforms.addComponent(second);
	Render& r2 = _renders.addComponent(second);
	r2.mesh = Primitive::createCylinderMesh(24);
	Primitive::InitBinds(_gfx, r2, t2);

}



void SceneManager::Update(float time)
{

	float angle = time;
	_transforms.getComponent(0).rotation = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, angle, angle);
	angle = -angle;
	float x = 2.0f * Input::GetNormedX() - 1.0f;
	float y = 300;
	float z = 2.0f * Input::GetNormedY() - 1.0f;
	Transform& tr2 = _transforms.getComponent(1);
	tr2.position.x = x;
	tr2.position.z = z;
	tr2.rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle, 0.0f, angle);
	float speed = 0.01f;

	if (Input::LeftKeyDown())// moving in left mouse button pressed
	{
		_mainCamera.transform.position.x -= speed * Input::GetDeltaX();
		_mainCamera.transform.position.y += speed * Input::GetDeltaY();
		_mainCamera.transform.position.z = -4.0f;
		return;
	}
	if (false && Input::RightKeyDown())// rotating in left mouse button pressed
	{
		float cam_yaw = -speed * Input::GetDeltaX();
		float cam_pitch = -speed * Input::GetDeltaY();
		_mainCamera.transform.rotation = DirectX::XMQuaternionMultiply(_mainCamera.transform.rotation, DirectX::XMQuaternionRotationRollPitchYaw(cam_pitch, cam_yaw, 0.0f));
		return;
	}

}

void SceneManager::Frame()
{
	_mainCamera.OnFrame();
	_gfx.ClearBuffer(_mainCamera.background);
	for (auto& [entID, tr] : _transforms)
	{
		_orientationSystem.OnFrame(tr, _mainCamera.transform);
	}
	for (auto& [entID, r] : _renders)
	{
		if (!_transforms.hasComponent(entID))
			continue;
		_renderSystem.OnFrame(r, _gfx, _transforms.getComponent(entID));
	}

	_gfx.endFrame();
}
