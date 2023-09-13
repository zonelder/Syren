#include "App.h"
#include <cmath>
#include "../component/Transform.h"
#include "TransformUtils.h"
#include "Input.h"

const double PI = acos(-1.0);

App::App() :_wnd(800, 600, "engin win"),_sceneManager(_wnd){}


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

void App::Update()
{
	_sceneManager.Update(_time.peek());
}

void App::Frame() {

	_sceneManager.Frame();
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
