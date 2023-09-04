#pragma once
#include "IDrawable.h"
#include <vector>
#include <DirectXMath.h>
#include "BindComponent/IBindable.h"

class Box:IDrawable
{
public:
	Box(Graphics&);
	void Draw(Graphics&) override;

	void BindTransformBuffer();
	DirectX::XMMATRIX transform;

private:

	const std::vector<DirectX::XMVECTOR> _vertices = {
	{ -1.0f,-1.0f,-1.0f,},
	{ 1.0f,-1.0f,-1.0f, },
	{ -1.0f,1.0f,-1.0f, },
	{ 1.0f,1.0f,-1.0f,  },
	{ -1.0f,-1.0f,1.0f, },
	{ 1.0f,-1.0f,1.0f,  },
	{ -1.0f,1.0f,1.0f,  },
	{ 1.0f,1.0f,1.0f,   },
	};
	const std::vector<unsigned short> _indices = {
	0,2,1, 2,3,1,
	1,3,5, 3,7,5,
	2,6,3, 3,6,7,
	4,5,7, 4,7,6,
	0,4,2, 2,4,6,
	0,1,4, 1,5,4
	};

	struct ConstantBuffer2 {
		struct {
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 = {
		{
			{1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
			{1.0f,1.0f,0.0f},
			{0.0f,1.0f,1.0f},
		}
	};
	
	std::vector<std::unique_ptr<IBindable>> _binds;
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_pConstantBuffer;
};

