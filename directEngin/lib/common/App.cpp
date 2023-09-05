#include "App.h"
#include <cmath>
const double PI = acos(-1.0);

App::App() :_wnd(800, 600, "engin win"),_box1(_wnd.getGraphic()), _box2(_wnd.getGraphic())
{}


int App::init(){
	MSG msg;
	BOOL gResult;
	while (true) {
		//if processMessage has a value then it means than we wanna exit from app
		if (const auto ecode = Window::processMessage()) {
			return *ecode;
		}
		frame();// TODO better handle vector<servise> so inplementation of each servises can be separete from app class
	}

}

void App::frame() {
	const float c = sin(_time.peek()) / 2.0f + 0.5f;
	float cam_yaw = -PI*(2.0* _wnd.mouseHandler.getPosX() /_wnd.GetWidth() - 1);
	float cam_pitch = -PI * (2.0 * _wnd.mouseHandler.getPosY() / _wnd.GetHeight() - 1);

	DirectX::XMMATRIX CameraTransform = 

		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f)*
		DirectX::XMMatrixRotationX(cam_pitch) *//TODO rework this part to rotate by Quaternions
		DirectX::XMMatrixRotationY(cam_yaw)*
		DirectX::XMMatrixPerspectiveFovLH(1.0f, _wnd.GetWidth() / _wnd.GetHeight(), 0.5f, 10.0f);// camera like on cords (0,0,-4)
	_wnd.getGraphic().clearBuffer(c, c, 1.0f);

	float angle = -_time.peek();
	_box1.transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixRotationY(angle) *
		CameraTransform

	);
	_box1.Draw(_wnd.getGraphic());
	angle = -angle;
	float x = 2.0f * _wnd.mouseHandler.getPosX() / 800.0f - 1.0f;
	float y = 300;
	float z = 2.0f * _wnd.mouseHandler.getPosY() / 600.0f - 1.0f;
	_box2.transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixRotationY(angle) *
		DirectX::XMMatrixTranslation(x, 0.0f, z) *
		CameraTransform
	);
	_box2.Draw(_wnd.getGraphic());

	_wnd.getGraphic().endFrame();
}
