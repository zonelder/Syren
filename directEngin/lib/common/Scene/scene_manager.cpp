#include "scene_manager.h"
#include "components/parent.h"


SceneManager::SceneManager(const Window& wnd):_input(wnd.inputHandler)
{
}


const Entity& SceneManager::createEntity() noexcept
{
	return _entityManager.create();
}

bool SceneManager::destroyEntity(const Entity& entt) noexcept
{
	return destroyEntity(entt.getID());
}

bool SceneManager::destroyEntity(EntityID entt_id) noexcept
{

	bool res = _entityManager.hasEntity(entt_id);
	if (!res) return false;
	const Entity& entt = _entityManager.get(entt_id);


	for (auto it = entt.begin(); it != entt.end(); ++it)
	{

		_ComponentManager.removeComponent(*it, entt_id);

	}

	_entityManager.destroy(entt);
	std::cout << "[Scene]Destroy entity " << entt_id << std::endl;

	return true;
}


void SceneManager::onStartFrame()
{
	_mainCamera.OnFrame();
}

void SceneManager::onEndFrame()
{}

const Entity& SceneManager::getEntity(EntityID id) const noexcept
{
	return _entityManager.get(id);
}

bool SceneManager::instantiate(FbxPrefabPtr pPrefab)
{
	if (!pPrefab)
		return false;

	std::unordered_map<int, EntityID> indexToID;
	int index = 0;
	for (auto& node : pPrefab->getNodes())
	{
		const Entity& entt = createEntity();
		indexToID[index] = entt.getID();
		++index;
		auto& transform = addComponent<Transform>(entt);
		transform.position = node.position;
		transform.rotation = DirectX::XMVectorSet(node.rotation.x, node.rotation.y, node.rotation.z, node.rotation.w);
		//transform.scale = node.scale;

		if (node.meshID != -1)
		{
			auto& render = addComponent<Render>(entt);
			render.pMesh = pPrefab->getMesh(node.meshID);
			render.pMaterial = SceneContext::pResources()->getMaterial("resource/example/tile_test/material/tile_black.syrenmaterial");//some default material
		}

		if (node.parent != -1)
		{
			auto& parent = addComponent<Parent>(entt);
			parent.parent = indexToID[node.parent];
		}
	}
	return true;
}

Camera& SceneManager::getCamera() noexcept
{
	return _mainCamera;
}

void SceneManager::updateInput(const Window& wnd) noexcept
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

const Input& SceneManager::getInput() const noexcept
{

	return _input;
}

MeshPtr SceneManager::make2SidedPlaneMesh()
{
	MeshPtr mesh = std::make_shared<Mesh>();
	mesh->vertexes = 
	{
		{{ -1.0f,-1.0f,0.0f,},{0.0f,1.0f}},
		{{ -1.0f,1.0f,0.0f, },{0.0f,0.0f}},
		{{ 1.0f,1.0f,0.0f,  },{1.0f,0.0f}},
		{{  1.0f,-1.0f,0.0f, },{1.0f,1.0f}},

		{{ -1.0f,-1.0f,0.0f,},{0.0f,1.0f}},
		{{ -1.0f,1.0f,0.0f, },{0.0f,0.0f}},
		{{ 1.0f,1.0f,0.0f,  },{1.0f,0.0f}},
		{{  1.0f,-1.0f,0.0f, },{1.0f,1.0f}},
	};
	mesh->indices =
	{
	0,2,1, 0,3,2,
	4,5,6, 4,6,7,
	};

	meshHelpers::updateBB(mesh.get());
	return mesh;
}


SceneContext* SceneContext::s_pMainContext = nullptr;
std::vector<SceneContext*> SceneContext::s_contexts;

SceneContext::SceneContext(ResourceManager* pRes, Graphics* pGfx) :
	_pResourceManager(pRes),
	_pGraphics(pGfx)
{
	assert(pRes && pGfx);
	if (s_contexts.empty())
		s_pMainContext = this;
	s_contexts.push_back(this);
}

SceneContext::~SceneContext()
{
	auto it = std::find(s_contexts.begin(), s_contexts.end(), this);
	if(it != s_contexts.end())
		s_contexts.erase(it);
	if(this == s_pMainContext && !s_contexts.empty())
		s_pMainContext = s_contexts.back();
}

