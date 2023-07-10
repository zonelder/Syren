#include "App.h"



App::App() :_wnd(800, 600, "engin win") {}


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
	_wnd.getGraphic().drawTestTriangle(_time.peek());
	_wnd.getGraphic().endFrame();
}
