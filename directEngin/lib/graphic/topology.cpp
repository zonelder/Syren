#include "pch.h"
#include "Topology.h"


Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	:
	p_type(type)
{}


void Topology::bind(Graphics& gfx) noexcept
{
	gfx.getContext()->IASetPrimitiveTopology(p_type);
}

