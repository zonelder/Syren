#pragma once
#include "IBindable.h"

class Topology:public IBindable
{
public:
	Topology() = default;
	Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	void bind(Graphics& gfx) noexcept override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY p_type{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

};

