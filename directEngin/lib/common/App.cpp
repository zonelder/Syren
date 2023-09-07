#include "App.h"
#include <cmath>
#include "../component/Transform.h"
#include "TransformUtils.h"

const double PI = acos(-1.0);

App::App() :_wnd(800, 600, "engin win"),_gfx(_wnd.GetHWND()), _box1(_gfx), _box2(_gfx)
{
	_mainCamera.aspectRatio = _wnd.GetWidth() / _wnd.GetHeight();
}


int App::Init(){
	MSG msg;
	BOOL gResult;
	while (true) {
		//if processMessage has a value then it means than we wanna exit from app
		if (const auto ecode = Window::processMessage()) {
			return *ecode;
		}

		Update();
		Frame();// TODO better handle vector<servise> so inplementation of each servises can be separete from app class
	}

}

// App может сама холдить сообщения о мыше и всяком таком говне и сохранять их где то в отдельном иснтансе к которому будет глобальный доступ ===))))))

void App::Update()
{
	float angle = _time.peek();
	_box1.transform.rotation = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, angle, angle);
	angle = -angle;
	float x = 2.0f * _wnd.mouseHandler.getPosX() / 800.0f - 1.0f;
	float y = 300;
	float z = 2.0f * _wnd.mouseHandler.getPosY() / 600.0f - 1.0f;
	_box2.transform.position.x = x;
	_box2.transform.position.z = z;
	_box2.transform.rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle, 0.0f, angle);


	//TODO MouseButtons pressed at the App's start. neet Fix
	if (_wnd.mouseHandler.LeftIsPressed())// moving in left mouse button pressed
	{
		//_mainCamera.transform.position = DirectX::XMFLOAT3{ 0.0f, 0.0f, -4.0f };
		float cam_x_delta = 10 * (2.0 * _wnd.mouseHandler.getPosX() / _wnd.GetWidth() - 1);
		float cam_y_delta = -5 * (2.0 * _wnd.mouseHandler.getPosY() / _wnd.GetHeight() - 1);// mult with -1 cause Y-axis invertedmos
		_mainCamera.transform.position = { cam_x_delta,cam_y_delta,-4.0f };
		return;
	}
	if (_wnd.mouseHandler.RightIsPressed())// rotating in left mouse button pressed
	{
		float cam_yaw = -PI * (2.0 * _wnd.mouseHandler.getPosX() / _wnd.GetWidth() - 1);
		float cam_pitch = -PI * (2.0 * _wnd.mouseHandler.getPosY() / _wnd.GetHeight() - 1);
		_mainCamera.transform.rotation = DirectX::XMQuaternionRotationRollPitchYaw(cam_pitch, cam_yaw, 0.0f);
		return;
	}

	/* // rotate camera with mouse
	float cam_yaw = -PI * (2.0 * _wnd.mouseHandler.getPosX() / _wnd.GetWidth() - 1);
	float cam_pitch = -PI * (2.0 * _wnd.mouseHandler.getPosY() / _wnd.GetHeight() - 1);
	_mainCamera.transform.rotation = DirectX::XMQuaternionRotationRollPitchYaw(cam_pitch, cam_yaw, 0.0f);
	*/
}

void App::Frame() {

	_mainCamera.OnFrame();
	_gfx.ClearBuffer(_mainCamera.background);


	_box1.orientationMatrix = DirectX::XMMatrixTranspose(
		toOrientationMatrix(_box1.transform)*
		_mainCamera.orientationMatrix

	);
	_box1.Draw(_gfx);

	_box2.orientationMatrix = DirectX::XMMatrixTranspose(
		toOrientationMatrix(_box2.transform) *
		_mainCamera.orientationMatrix
	);
	_box2.Draw(_gfx);

	_gfx.endFrame();
}
