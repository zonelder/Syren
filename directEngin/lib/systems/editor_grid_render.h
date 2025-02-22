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
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_colorConstantBuffer;
	VertexConstantBuffer<DirectX::XMMATRIX> _vertexConstantBuffer;//for transform
	VertexBuffer _vertexBuffer;//for vert
	std::vector<Vertex> vertices_;


	std::shared_ptr<Material> _pMaterial{ nullptr };
	Topology _topology;
};



#endif // ! __EDITOR_GREED_RENDERER__
