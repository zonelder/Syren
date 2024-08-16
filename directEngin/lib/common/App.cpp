#include "App.h"
#include <cmath>
#include "../component/Transform.h"
#include "TransformUtils.h"
#include "Input.h"




App::App() :_wnd(800, 600, "engin win"),_scene(_wnd){}


int App::Init(){

	OnInit();
	_systemManager.init(_scene);
	while (true) {
		//if processMessage has a value then it means than we wanna exit from app
		if (const auto ecode = Window::processMessage()) {
			return *ecode;
		}
		_scene.updateInput(_wnd);
		Update();
		Frame();// TODO better handle vector<servise> so inplementation of each servises can be separete from app class
	}

}

void App::Update()
{


	_systemManager.update(_scene, _time.peek());

}

void App::Frame() {


	_scene.onStartFrame();


	_systemManager.frame(_scene);


	_scene.onEndFrame();

}


void App::SetInputData()
{


}
