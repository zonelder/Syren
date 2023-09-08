#include "Primitive.h"
#include "BindComponent/BindableComponents.h"



Primitive::Primitive() :orientationMatrix(DirectX::XMMatrixIdentity()){}

Primitive::Primitive(Mesh& _mesh):mesh(_mesh),orientationMatrix(DirectX::XMMatrixIdentity()) {}


void Primitive::InitBinds(Graphics& gfx)
{
	_binds.clear();
	auto vertexBuffer = std::make_unique<VertexBuffer>(gfx, mesh.vertices);
	auto indexBuffer = std::make_unique<IndexBuffer>(gfx, mesh.indices);

	auto vertexConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx, orientationMatrix);
	p_pConstantBuffer = vertexConstantBuffer->p_pConstantBuffer;

	auto pixelConstantBuffer = std::make_unique<PixelConstantBuffer<Mesh::ConstantBuffer2>>(gfx, mesh.colors);
	auto pixelShader = std::make_unique<PixelShader>(gfx, L"PixelShader.cso");
	auto vertexshader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	ID3DBlob* pBlob = vertexshader->getBytecode();

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
	{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	auto inputLayout = std::make_unique<InputLayout>(gfx, ied, pBlob);

	auto topology = std::make_unique <Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	_binds.push_back(std::move(vertexConstantBuffer));
	_binds.push_back(std::move(vertexBuffer));
	_binds.push_back(std::move(indexBuffer));
	_binds.push_back(std::move(pixelConstantBuffer));
	_binds.push_back(std::move(pixelShader));
	_binds.push_back(std::move(vertexshader));
	_binds.push_back(std::move(inputLayout));
	_binds.push_back(std::move(topology));
}


void Primitive::Draw(Graphics& gfx)
{
	INFOMAN(gfx);

	///update transform buffer
	D3D11_MAPPED_SUBRESOURCE msr;
	GFX_THROW_INFO(gfx.getContext()->Map(
		p_pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	));
	memcpy(msr.pData, &orientationMatrix, sizeof(orientationMatrix));
	gfx.getContext()->Unmap(p_pConstantBuffer.Get(), 0u);


	// use binds
	for (auto& bind : _binds)
	{
		bind->bind(gfx);
	}

	//draw
	gfx.DrawIndexed(mesh.indices);
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
