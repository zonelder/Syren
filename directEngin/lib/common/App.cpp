#include "App.h"
#include <cmath>
#include "../component/Transform.h"
#include "TransformUtils.h"
#include "Input.h"

const double PI = acos(-1.0);

App::App() :_wnd(800, 600, "engin win"),_gfx(_wnd.GetHWND())
{
	_mainCamera.aspectRatio = _wnd.GetWidth() / _wnd.GetHeight();

	EntityID first = 0;
	_transforms[first] = Transform();
	_renders[first] = Render();
	_renders[first].mesh = Primitive::CreateBoxMesh();
	Primitive::InitBinds(_gfx, _renders.at(first), _transforms.at(first));

	EntityID second = 1;
	_transforms[second] = Transform();
	_renders[second] = Render();
	_renders[second].mesh = Primitive::createCylinderMesh(24);
	Primitive::InitBinds(_gfx, _renders.at(second), _transforms.at(second));

}


int App::Init(){
	MSG msg;
	BOOL gResult;


	while (true) {
		//if processMessage has a value then it means than we wanna exit from app
		if (const auto ecode = Window::processMessage()) {
			return *ecode;
		}
		SetInputData();
		Update();
		Frame();// TODO better handle vector<servise> so inplementation of each servises can be separete from app class
	}

}

// App может сама холдить сообщения о мыше и всяком таком говне и сохранять их где то в отдельном иснтансе к которому будет глобальный доступ ===))))))

void App::Update()
{

	float angle = _time.peek();
	_transforms.at(0).rotation = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, angle, angle);
	angle = -angle;
	float x = 2.0f * Input::GetNormedX() - 1.0f;
	float y = 300;
	float z = 2.0f * Input::GetNormedY() - 1.0f;
	Transform& tr2 = _transforms.at(1);
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
	if (false &&  Input::RightKeyDown())// rotating in left mouse button pressed
	{
		float cam_yaw = -speed * Input::GetDeltaX();
		float cam_pitch = -speed * Input::GetDeltaY();
		_mainCamera.transform.rotation = DirectX::XMQuaternionMultiply(_mainCamera.transform.rotation, DirectX::XMQuaternionRotationRollPitchYaw(cam_pitch, cam_yaw, 0.0f));
		return;
	}

}

void App::Frame() {

	_mainCamera.OnFrame();
	_gfx.ClearBuffer(_mainCamera.background);

	for (auto& [entID, tr] : _transforms)
	{
		_orientationSystem.OnFrame(tr, _mainCamera.transform);
	}
	for (auto& [entID, r] : _renders)
	{
		if (!_transforms.contains(entID))
			continue;
		_renderSystem.OnFrame(r, _gfx, _transforms.at(entID));
	}

	_gfx.endFrame();
}


void App::SetInputData()
{
	MouseHandler& mh = _wnd.mouseHandler;
	InputHandler& ih = _wnd.inputHandler;
	Input& m = Input::GetInstance();
	int x = mh.getPosX();
	int y = mh.getPosY();
	m._dx = x - m._x;
	m._dy = y - m._y;
	m._x = x;
	m._y = y;

	m._clampX = (float)x / _wnd.GetWidth();
	m._clampY = (float)y / _wnd.GetHeight();

	m._isLeftPressed = mh.LeftIsPressed();
	m._isRightPressed = mh.RightIsPressed();

}
