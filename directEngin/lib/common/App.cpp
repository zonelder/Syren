#include "App.h"
#include <cmath>
#include "../Components/Transform.h"
const double PI = acos(-1.0);


DirectX::XMMATRIX calcOrientation(const Transform& tr)
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&tr.position);
	return DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&tr.scale), { 0.0f,0.0f,0.0f,0.0f }, tr.rotation, pos);
}


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
		Update();
		frame();// TODO better handle vector<servise> so inplementation of each servises can be separete from app class
	}

}

void App::Update(float dt)
{
	const float c = sin(_time.peek()) / 2.0f + 0.5f;
	float angle = -_time.peek();
	_box1.transform.rotation = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, angle, angle);
	angle = -angle;
	float x = 2.0f * _wnd.mouseHandler.getPosX() / 800.0f - 1.0f;
	float y = 300;
	float z = 2.0f * _wnd.mouseHandler.getPosY() / 600.0f - 1.0f;
	_box2.transform.position.x = x;
	_box2.transform.position.z = z;
	_box2.transform.rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle, angle, 0.0f);
}

void App::frame() {
	float cam_yaw = -PI*(2.0* _wnd.mouseHandler.getPosX() /_wnd.GetWidth() - 1);
	float cam_pitch = -PI * (2.0 * _wnd.mouseHandler.getPosY() / _wnd.GetHeight() - 1);
	Transform cameraTransfom;
	cameraTransfom.position = DirectX::XMFLOAT3{ 0.0f, 0.0f, -4.0f};
	//cameraTransfom.rotation = DirectX::XMQuaternionRotationRollPitchYaw(cam_pitch, cam_yaw, 0.0f);
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&cameraTransfom.position);
	DirectX::XMMATRIX CameraTransformMartix = 
		DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&cameraTransfom.scale), pos, cameraTransfom.rotation,DirectX::XMVectorNegate(pos))*
		DirectX::XMMatrixPerspectiveFovLH(1.0f, _wnd.GetWidth() / _wnd.GetHeight(), 0.5f, 10.0f);// camera like on cords (0,0,-4)

	_wnd.getGraphic().clearBuffer(1.0f,1.0f, 1.0f);//set background  color

	_box1.i_orientationMatrix = DirectX::XMMatrixTranspose(
		calcOrientation(_box1.transform)*
		CameraTransformMartix
	);
	_box1.Draw(_wnd.getGraphic());;

	_box2.i_orientationMatrix = DirectX::XMMatrixTranspose(
		calcOrientation(_box2.transform) *
		CameraTransformMartix
	);
	_box2.Draw(_wnd.getGraphic());

	_wnd.getGraphic().endFrame();
}
