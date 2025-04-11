#include "App.h"
#include <cmath>
#include "components/transform.h"
#include "input.h"




App::App() :
	_wnd(800, 600, "engin win"),
	_context(_wnd)
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
		_context.updateInput(_wnd);//it will be better to have callback in Window class
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
	auto gfx = _context.pGfx();

	gfx->ClearBuffer(_scene.getCamera().background);
	_scene.onStartFrame();

	_systemManager.frame(_scene);

	_scene.onEndFrame();

	gfx->endFrame();
}
