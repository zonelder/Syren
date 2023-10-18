#include "Mesh.h"



void Mesh::init(Graphics& gfx)
{
	if (isInit) return;

	vertexBuffer = VertexBuffer(gfx, vertices);
	indexBuffer = IndexBuffer(gfx, indices);
	pixelConstantBuffer = PixelConstantBuffer<Mesh::ConstantBuffer2>(gfx, colors);
	isInit = true;
}

void Mesh::bind(Graphics& gfx)
{
	vertexBuffer.bind(gfx);
	indexBuffer.bind(gfx);
	pixelConstantBuffer.bind(gfx);
}