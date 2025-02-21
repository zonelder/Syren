#ifndef  __EDITOR_GRID_RENDERER__
#define __EDITOR_GRID_RENDERER__


#include "common/Scene/base_system.h"
#include "components/material.h"
#include <vector>

class EditorGridRenderer : public BaseSystem
{
public:
	void onInit(SceneManager& scene) override;
	void onFrame(SceneManager& scene) override;

private:
	DirectX::XMMATRIX _wvp;
	VertexConstantBuffer<DirectX::XMMATRIX> _vertexConstantBuffer;//for transform
	VertexBuffer _vertexBuffer;//for vert
	std::vector<VertexXYZD> vertices_;

	//for vertex shader// has to be unified
	Microsoft::WRL::ComPtr<ID3D11VertexShader> p_pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> p_pBytecodeBlob;
	InputLayout _inputLayer;

	//pixel Shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> p_pPixelShader{ nullptr };
};



#endif // ! __EDITOR_GREED_RENDERER__
