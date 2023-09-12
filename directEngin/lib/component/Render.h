#pragma once
#include "Mesh.h"
#include <vector>
#include "../graphics/Drawable/BindComponent/IBindable.h"

struct Render
{
	Mesh mesh;
	std::vector<std::unique_ptr<IBindable>> pBinds;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};
