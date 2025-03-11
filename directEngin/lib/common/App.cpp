#include "App.h"
#include <cmath>
#include "components/transform.h"
#include "transform_utils.h"
#include "input.h"




App::App() :
	_wnd(800, 600, "engin win"),
	_gfx(_wnd.GetHWND()),
	_rm(_gfx),
	_scene(_wnd),
	_context(&_rm, &_gfx)
{}


int App::Init()
{

	OnInit();
	_systemManager.init(_scene);
	while (true) {
		//if processMessage has a value then it means than we wanna exit from app
		if (const auto ecode = Window::processMessage()) 
		{
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

void App::Frame() 
{

	_gfx.ClearBuffer(_scene.getCamera().background);
	_scene.onStartFrame();


	_systemManager.frame(_scene);


	_scene.onEndFrame();
	_gfx.endFrame();
}


void App::SetInputData()
{

}
