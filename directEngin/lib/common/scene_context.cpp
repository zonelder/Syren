#include "scene_context.h"
#include "scene_context.h"


SceneContext* SceneContext::s_pMainContext = nullptr;
std::vector<SceneContext*> SceneContext::s_contexts;

SceneContext::SceneContext(const Window& wnd) : 
	_pGraphics(wnd.GetHWND()),
	_pResourceManager(_pGraphics),
	_input(wnd.inputHandler)
{
	if (s_contexts.empty())
		s_pMainContext = this;
	s_contexts.push_back(this);
}


SceneContext::~SceneContext()
{
	auto it = std::find(s_contexts.begin(), s_contexts.end(), this);
	if (it != s_contexts.end())
		s_contexts.erase(it);
	if (this == s_pMainContext && !s_contexts.empty())
		s_pMainContext = s_contexts.back();
}


//---------------------context shortcuts-------------------
ResourceManager& context::rm()
{
	return SceneContext::resources();
}

Graphics& context::gfx()
{
	return *(SceneContext::pGfx());
}

Input& context::input()
{
	return SceneContext::input();
}

TexturePtr context::getTexture(const std::string& path)
{
	return SceneContext::resources().getTexture(path);
}

MeshPtr context::getMesh(const std::string& path)
{
	return SceneContext::resources().getMesh(path);
}

MaterialPtr context::getMaterial(const std::string& path)
{
	return SceneContext::resources().getMaterial(path);
}

VertexShaderPtr context::getVertexShader(const std::string& path)
{
	return SceneContext::resources().getVertexShader(path);
}

PixelShaderPtr context::getPixelShader(const std::string& path)
{
	return SceneContext::resources().getPixelShader(path);
}


void SceneContext::updateInput(const Window& wnd)
{

	const MouseHandler& mh = wnd.mouseHandler;
	const InputHandler& ih = wnd.inputHandler;
	int x = mh.getPosX();
	int y = mh.getPosY();
	_input.deltaX = x - _input.x;
	_input.deltaY = y - _input.y;
	_input.x = x;
	_input.y = y;

	_input.normedX = (float)x / wnd.GetWidth();
	_input.normedY = (float)y / wnd.GetHeight();

	_input.isLeftPressed = mh.LeftIsPressed();
	_input.isRightPressed = mh.RightIsPressed();

}
