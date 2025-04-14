#include "App.h"
#include "components/transform.h"
#include "components/render.h"
#include "components/parent.h"
#include "core/input.h"


std::unordered_map<int, EntityID> instantiate(SceneManager& scene,FbxPrefabPtr pPrefab)
{
	if (!pPrefab)
		return {};

	std::unordered_map<int, EntityID> indexToID;
	int index = 0;
	for (auto& node : pPrefab->getNodes())
	{
		const Entity& entt = scene.createEntity();
		indexToID[index] = entt.getID();
		++index;
		auto& transform = scene.addComponent<Transform>(entt);
		transform.position = node.position;
		transform.rotation = node.rotation;
		//transform.scale = node.scale;

		if (node.meshID != -1)
		{
			auto& render = scene.addComponent<Render>(entt);
			render.pMesh = pPrefab->getMesh(node.meshID);
			render.pMaterial = context::getMaterial("resource/example/tile_test/material/tile_black.syrenmaterial");//some default material
		}

		if (node.parent != -1)
		{
			auto& parent = scene.addComponent<Parent>(entt);
			parent.parent = indexToID[node.parent];
		}
	}
	return indexToID;
}

App::App() :
	_wnd(800, 600, L"engin win"),
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
