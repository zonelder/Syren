#include "Box.h"
#include "BindComponent/BindableComponents.h"



Box::Box(Graphics& gfx):transform(DirectX::XMMatrixIdentity())
{

	auto vertexBuffer = std::make_unique<VertexBuffer>(gfx, _vertices);
	auto indexBuffer = std::make_unique<IndexBuffer>(gfx, _indices);
	transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f) *
		DirectX::XMMatrixPerspectiveFovLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
	);
	auto vertexConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx,transform);
	p_pConstantBuffer = vertexConstantBuffer->p_pConstantBuffer;

	transform *= DirectX::XMMatrixRotationZ(15)* DirectX::XMMatrixRotationY(15);

	//vertexConstantBuffer->update(gfx, transform);
	auto pixelConstantBuffer = std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2);
	auto pixelShader = std::make_unique<PixelShader>(gfx, L"PixelShader.cso");
	auto vertexshader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	ID3DBlob* pBlob = vertexshader->getBytecode();

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
	{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	auto inputLayout = std::make_unique<InputLayout>(gfx, ied, pBlob);

	auto topology = std::make_unique <Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	_binds.push_back(std::move(vertexConstantBuffer)); // пусть первым будет всегда оно
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

	D3D11_MAPPED_SUBRESOURCE msr;
	GFX_THROW_INFO(gfx.getContext()->Map(
		p_pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	));
	memcpy(msr.pData, &transform, sizeof(transform));
	gfx.getContext()->Unmap(p_pConstantBuffer.Get(), 0u);

	for (auto& bind : _binds)
	{
		bind->bind(gfx);
	}


	gfx.DrawIndexed(_indices);
}

