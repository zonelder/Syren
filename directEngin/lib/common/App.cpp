#include "App.h"



App::App() :_wnd(800, 300, "engin win") {}


int App::init(){
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		update();// TODO better handle vector<servise> so inplementation of each servises can be separete from app class
	}

	if (gResult == -1) {
		throw WND_LAST_EXCEPT();
	}

	return msg.wParam;
}

void App::update() {
	const float t = _time.peek();
	std::ostringstream oss;
	oss << "Time elapsed:" << std::setprecision(1) << std::fixed << t << std::endl;
	_wnd.SetTitle(oss.str());
}
