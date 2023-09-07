#include "App.h"
#include <cmath>
#include "../component/Transform.h"
#include "TransformUtils.h"

const double PI = acos(-1.0);

App::App() :_wnd(800, 600, "engin win"),_gfx(_wnd.GetHWND()), _box1(_gfx), _box2(_gfx)
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
	Transform cameraTransfom;
	cameraTransfom.position = DirectX::XMFLOAT3{ 0.0f, 0.0f, -4.0f };
//	cameraTransfom.rotation = DirectX::XMQuaternionRotationRollPitchYaw(cam_pitch, cam_yaw, 0.0f);
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&cameraTransfom.position);
	DirectX::XMMATRIX CameraTransformMartix =
		DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&cameraTransfom.scale), pos, cameraTransfom.rotation, DirectX::XMVectorNegate(pos)) *
		DirectX::XMMatrixPerspectiveFovLH(1.0f, _wnd.GetWidth() / _wnd.GetHeight(), 0.5f, 10.0f);// camera like on cords (0,0,-4)
	_gfx.clearBuffer(c, c, 1.0f);

	float angle = -_time.peek();
	_box1.transform.rotation = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, angle, angle);
	_box1.orientationMatrix = DirectX::XMMatrixTranspose(
		toOrientationMatrix(_box1.transform)*
		CameraTransformMartix

	);
	_box1.Draw(_gfx);
	//dada
	angle = -angle;
	float x = 2.0f * _wnd.mouseHandler.getPosX() / 800.0f - 1.0f;
	float y = 300;
	float z = 2.0f * _wnd.mouseHandler.getPosY() / 600.0f - 1.0f;
	_box2.transform.position.x = x;
	_box2.transform.position.z = z;
	_box2.transform.rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle, 0.0f, angle);
	_box2.orientationMatrix = DirectX::XMMatrixTranspose(
		toOrientationMatrix(_box2.transform) *
		DirectX::XMMatrixTranslation(x, 0.0f, z) *
		CameraTransformMartix
	);
	_box2.Draw(_gfx);

	_gfx.endFrame();
}
