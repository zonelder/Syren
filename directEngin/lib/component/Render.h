#pragma once
#include "Mesh.h"
#include <vector>
#include <string>
#include "../graphics/Drawable/BindComponent/BindableComponents.h"
#include <memory>

struct Render
{
	std::shared_ptr<Mesh> p_mesh;

	//std::string vertexShader = "VertexShader.cso";
	//std::string pixelShader  = "PixelShader.cso";

	VertexConstantBuffer<DirectX::XMMATRIX> vertexConstantBuffer;
	PixelShader pixelShader;
	VertexShader vertexShader;
	InputLayout inputLayer;
	Topology topology;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};
