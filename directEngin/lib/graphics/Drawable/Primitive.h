#pragma once
#include "IDrawable.h"
#include <vector>
#include <DirectXMath.h>
#include "BindComponent/IBindable.h"
#include "../../component/Transform.h"
#include "../../component/Mesh.h"

class Primitive:IDrawable
{
public:
	Primitive();
	Primitive(Mesh&);

	/// @brief reset all binds and build new 
	void InitBinds(Graphics&);
	void Draw(Graphics&) override;


	static Primitive CreateBox();
	static Primitive createCylinder();

	/// @brief Primitive object mesh data. DO NOT CHANGE THIS DATA STRUCTURE
	Mesh mesh;
	Transform transform;
	DirectX::XMMATRIX orientationMatrix;

private:

	std::vector<std::unique_ptr<IBindable>> _binds;
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_pConstantBuffer;
};

