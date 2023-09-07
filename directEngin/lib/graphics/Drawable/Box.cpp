#include "Box.h"
#include "BindComponent/BindableComponents.h"



Box::Box(Graphics& gfx):orientationMatrix(DirectX::XMMatrixIdentity())
{

	auto vertexBuffer = std::make_unique<VertexBuffer>(gfx, _vertices);
	auto indexBuffer = std::make_unique<IndexBuffer>(gfx, _indices);

	auto vertexConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx,orientationMatrix);
	p_pConstantBuffer = vertexConstantBuffer->p_pConstantBuffer;

	auto pixelConstantBuffer = std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2);
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


void Box::Draw(Graphics& gfx)
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
	gfx.DrawIndexed(_indices);
}

