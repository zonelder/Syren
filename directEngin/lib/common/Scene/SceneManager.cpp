#include "SceneManager.h"



SceneManager::SceneManager(const Window& wnd):_gfx(wnd.GetHWND()){}



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