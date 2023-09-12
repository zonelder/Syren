#include "Primitive.h"
#include "BindComponent/BindableComponents.h"



Primitive::Primitive(){}

Primitive::Primitive(Mesh& _mesh)
{

	render.mesh = _mesh;
}


void Primitive::InitBinds(Graphics& gfx,Render& r,Transform& tr)
{
	r.pBinds.clear();
	auto vertexBuffer = std::make_unique<VertexBuffer>(gfx, r.mesh.vertices);
	auto indexBuffer = std::make_unique<IndexBuffer>(gfx, r.mesh.indices);

	auto vertexConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx,tr.orientationMatrix);
	r.pConstantBuffer = vertexConstantBuffer->p_pConstantBuffer;

	auto pixelConstantBuffer = std::make_unique<PixelConstantBuffer<Mesh::ConstantBuffer2>>(gfx, r.mesh.colors);
	auto pixelShader = std::make_unique<PixelShader>(gfx, L"PixelShader.cso");
	auto vertexshader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	ID3DBlob* pBlob = vertexshader->getBytecode();

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
	{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	auto inputLayout = std::make_unique<InputLayout>(gfx, ied, pBlob);

	auto topology = std::make_unique <Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	r.pBinds.push_back(std::move(vertexConstantBuffer));
	r.pBinds.push_back(std::move(vertexBuffer));
	r.pBinds.push_back(std::move(indexBuffer));
	r.pBinds.push_back(std::move(pixelConstantBuffer));
	r.pBinds.push_back(std::move(pixelShader));
	r.pBinds.push_back(std::move(vertexshader));
	r.pBinds.push_back(std::move(inputLayout));
	r.pBinds.push_back(std::move(topology));
}



Primitive Primitive::CreateBox()
{
	Mesh mesh;
	mesh.vertices = {
	{ -1.0f,-1.0f,-1.0f,},
	{ 1.0f,-1.0f,-1.0f, },
	{ -1.0f,1.0f,-1.0f, },
	{ 1.0f,1.0f,-1.0f,  },
	{ -1.0f,-1.0f,1.0f, },
	{ 1.0f,-1.0f,1.0f,  },
	{ -1.0f,1.0f,1.0f,  },
	{ 1.0f,1.0f,1.0f,   },
	};
	mesh.indices = {
	0,2,1, 2,3,1,
	1,3,5, 3,7,5,
	2,6,3, 3,6,7,
	4,5,7, 4,7,6,
	0,4,2, 2,4,6,
	0,1,4, 1,5,4
	};
	mesh.colors = {
		{
			{1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
			{1.0f,1.0f,0.0f},
			{0.0f,1.0f,1.0f},
		}
	};

	Primitive obj = Primitive(mesh);
	return obj;
}


Primitive Primitive::createCylinder(unsigned int n)
{
	Mesh mesh;
	float two_pi = 6.28318530718f;
	float angl;
	mesh.vertices.reserve(n + 2);
	mesh.indices.reserve(6 * n);
	for (unsigned int k = 0; k < n;++k)
	{
		angl = (two_pi * k) / n;
		mesh.vertices.push_back({ cos(angl),0.0f,sin(angl) });

		mesh.indices.push_back(k);
		mesh.indices.push_back(n);
		mesh.indices.push_back((k + 1) % n);

		mesh.indices.push_back(k);
		mesh.indices.push_back((k + 1) % n);
		mesh.indices.push_back(n+1);


	}
	mesh.vertices.push_back({ 0.0f,1.0f,0.0f });
	mesh.vertices.push_back({ 0.0f,0.0f,0.0f });

	mesh.colors = {
		{
			{1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
			{1.0f,1.0f,0.0f},
			{0.0f,1.0f,1.0f},
		}
	};

	Primitive obj = Primitive(mesh);
	return obj;
}
