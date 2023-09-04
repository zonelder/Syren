#include "App.h"



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
	_wnd.getGraphic().clearBuffer(c, c, 1.0f);

	float angle = -_time.peek();
	_box1.transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixRotationY(angle) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f) *
		DirectX::XMMatrixPerspectiveFovLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
	);
	_box1.Draw(_wnd.getGraphic());
	angle = -angle;
	float x = 2.0f * _wnd.mouseHandler.getPosX() / 800.0f - 1.0f;
	float y = 300;
	float z = 2.0f * _wnd.mouseHandler.getPosY() / 600.0f - 1.0f;
	_box2.transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixRotationY(angle) *
		DirectX::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
		DirectX::XMMatrixPerspectiveFovLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
	);
	_box2.Draw(_wnd.getGraphic());

	_wnd.getGraphic().endFrame();
}
