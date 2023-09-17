#pragma once
#include "Mesh.h"
#include <vector>
#include <string>
#include "../graphics/Drawable/BindComponent/BindableComponents.h"

struct Render
{
	Mesh mesh;

	//std::string vertexShader = "VertexShader.cso";
	//std::string pixelShader  = "PixelShader.cso";

	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	VertexConstantBuffer<DirectX::XMMATRIX> vertexConstantBuffer;
	PixelConstantBuffer<Mesh::ConstantBuffer2> pixelConstantBuffer;
	PixelShader pixelShader;
	VertexShader vertexShader;
	InputLayout inputLayer;
	Topology topology;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};
