#include "App.h"
#include <cmath>
#include "components/transform.h"
#include "input.h"




App::App() :
	_wnd(800, 600, "engin win"),
	_context(_wnd)
{}

int App::init()
{

	onInit();
	_systemManager.init(_scene);
	while (true) 
	{
		//if processMessage has a value then it means than we wanna exit from app
		if (const auto ecode = Window::processMessage()) 
		{
			return ecode.exitCode;
		}
		_context.updateInput(_wnd);//it will be better to have callback in Window class
		update();
		frame();// TODO better handle vector<servise> so inplementation of each servises can be separete from app class
	}

}

void App::update()
{
	_systemManager.update(_scene, _time.peek());
}

void App::frame() 
{
	auto gfx = _context.pGfx();

	gfx->clearBuffer(_scene.getCamera().background);
	_scene.onStartFrame();

	_systemManager.frame(_scene);

	_scene.onEndFrame();

	gfx->endFrame();
}
