#include "SceneManager.h"
#include "../../component/TimedRotation.h"
#include "../../component/Parent.h"


SceneManager::SceneManager(const Window& wnd):_gfx(wnd.GetHWND()){
	_mainCamera.aspectRatio = wnd.GetWidth() / wnd.GetHeight();


	_ComponentManager.addPool<Parent>();
	_ComponentManager.addPool<Transform>();
	_ComponentManager.addPool<Render>();
	

	auto material = makeMaterial();
	/*
	EntityID first = 0;
	Transform& t = addComponent<Transform>(first);
	Render& r = addComponent<Render>(first);
	TimedRotation& tr = addComponent<TimedRotation>(first);

	tr.xSence = 0.0f;
	tr.ySence = 1.0f;
	tr.zSence = 1.0f;
	r.p_mesh = makeBoxMesh();
	r.p_material = material;
	 
	EntityID second = 1;
	Transform& t2 = addComponent<Transform>(second);
	Render& r2 = addComponent<Render>(second);
	TimedRotation& tr2 = addComponent<TimedRotation>(second);

	tr2.xSence = 1.0f;
	tr2.ySence = 0.0f;
	tr2.zSence = 1.0f;
	r2.p_mesh = makeCylinderMesh();
	r2.p_material = material;
	t2.scale.y = 3.0f;
	*/

	EntityID second = 1;
	auto p_plane_mesh = make2SidedPlaneMesh();
	EntityID id = second;
	Transform& t = addComponent<Transform>(id);
	Render& r = addComponent<Render>(id);
	Parent& p = addComponent<Parent>(id);
	TimedRotation& tr = addComponent<TimedRotation>(id);
	tr.zSence = 1.2f;
	p.parent = second + 1;
	r.p_mesh = p_plane_mesh;
	r.p_material = material;
	t.position = {0.0f,0.0f,1.0f};
	t.scale = { 1.0f,1.0f,1.0f };
	for (int i = 0; i < 1; ++i)
	{
		EntityID id = i + second + 1;
		Transform& t = addComponent<Transform>(id);
		Render& r = addComponent<Render>(id);
		TimedRotation& tr = addComponent<TimedRotation>(id);
		tr.zSence = 1.2f;
		tr.xSence = 1.0f;
		r.p_mesh = p_plane_mesh;
		r.p_material = material;
		t.position = { float(i % 10)*3.0f,0.0f,float(i / 10)*3.0f };
	}
}



void SceneManager::onStartFrame()
{
	_mainCamera.OnFrame();
	_gfx.ClearBuffer(_mainCamera.background);
}

void SceneManager::onEndFrame()
{
	_gfx.endFrame();
}


Camera& SceneManager::getCamera() noexcept
{
	return _mainCamera;
}

Graphics& SceneManager::getGraphic() noexcept
{
	return _gfx;
}


std::shared_ptr<Material> SceneManager::makeMaterial(const wchar_t* vertexShader, const wchar_t* pixelShader)
{
	auto mat = std::make_shared<Material>(_gfx, vertexShader, pixelShader);

	return mat;
}


std::shared_ptr<Mesh> SceneManager::makeMesh(
	const std::vector<Vertex>& vertices,
	const std::vector<unsigned short>& indices,
	const Mesh::ConstantBuffer2& colors
)
{
	auto mesh = std::make_shared<Mesh>(_gfx, vertices, indices,colors);
	return mesh;
}
/*
std::shared_ptr<Mesh>  SceneManager::makeBoxMesh()
{
	auto mesh = makeMesh({
	{ -1.0f,-1.0f,-1.0f,},
	{ 1.0f,-1.0f,-1.0f, },
	{ -1.0f,1.0f,-1.0f, },
	{ 1.0f,1.0f,-1.0f,  },
	{ -1.0f,-1.0f,1.0f, },
	{ 1.0f,-1.0f,1.0f,  },
	{ -1.0f,1.0f,1.0f,  },
	{ 1.0f,1.0f,1.0f,   },
		},
	{
	0,2,1, 2,3,1,
	1,3,5, 3,7,5,
	2,6,3, 3,6,7,
	4,5,7, 4,7,6,
	0,4,2, 2,4,6,
	0,1,4, 1,5,4
	},
	{
		{
			{1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
			{1.0f,1.0f,0.0f},
			{0.0f,1.0f,1.0f},
		}
	}
	);
	return mesh;
}

std::shared_ptr<Mesh>  SceneManager::makeCylinderMesh(unsigned int n)
{
	std::vector<Vertex> vertices;
	vertices.reserve(n + 2);
	std::vector<unsigned short> indices;
	indices.reserve(6 * n);

	float two_pi = 6.28318530718f;
	float angl;
	for (unsigned int k = 0; k < n; ++k)
	{
		angl = (two_pi * k) / n;
		vertices.push_back({ cos(angl),0.0f,sin(angl) });

		indices.push_back(k);
		indices.push_back(n);
		indices.push_back((k + 1) % n);

		indices.push_back(k);
		indices.push_back((k + 1) % n);
		indices.push_back(n + 1);


	}
	vertices.push_back({ 0.0f,1.0f,0.0f });
	vertices.push_back({ 0.0f,0.0f,0.0f });

	Mesh::ConstantBuffer2 colors = {
		{
			{1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
			{1.0f,1.0f,0.0f},
			{0.0f,1.0f,1.0f},
		}
	};

	return makeMesh(vertices, indices, colors);
}
*/

std::shared_ptr<Mesh> SceneManager::make2SidedPlaneMesh()
{
	auto mesh = makeMesh({
		{{ -1.0f,-1.0f,0.0f,},{0.0f,1.0f}},
		{{ -1.0f,1.0f,0.0f, },{0.0f,0.0f}},
		{{ 1.0f,1.0f,0.0f,  },{1.0f,0.0f}},
		{{  1.0f,-1.0f,0.0f, },{1.0f,1.0f}},

		{{ -1.0f,-1.0f,0.0f,},{0.0f,1.0f}},
		{{ -1.0f,1.0f,0.0f, },{0.0f,0.0f}},
		{{ 1.0f,1.0f,0.0f,  },{1.0f,0.0f}},
		{{  1.0f,-1.0f,0.0f, },{1.0f,1.0f}},
		},
	{
	0,2,1, 0,3,2,
	4,5,6, 4,6,7,
	},
	{
		{
			{0.4f,1.0f,1.0f},
			{1.0f,0.0f,0.0f},
		}
	}
	);
	return mesh;
}