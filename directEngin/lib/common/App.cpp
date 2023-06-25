#include "App.h"



App::App() :_wnd(800, 300, "engin win") {}


int App::init(){
	MSG msg;
	BOOL gResult;
	while (true) {
		//if processMessage has a value then it means than we wanna exit from app
		if (const auto ecode = Window::processMessage()) {
			return *ecode;
		}
		update();// TODO better handle vector<servise> so inplementation of each servises can be separete from app class
	}

}

void App::update() {
	const float t = _time.peek();
	std::ostringstream oss;
	oss << "Time elapsed:" << std::setprecision(1) << std::fixed << t << std::endl;
	_wnd.SetTitle(oss.str());
}
