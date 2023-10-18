#pragma once
#include "../graphics/Drawable/BindComponent/VertexShader.h"
#include "../graphics/Drawable/BindComponent/PixelShader.h"
#include "../graphics/Drawable/BindComponent/InputLayout.h"

struct Material
{
public:
	void bind(Graphics& gfx);
	void init(Graphics& gfx);


	std::wstring vertexShader;
	std::wstring pixelShader;
private:
	VertexShader _vertexShader;
	PixelShader _pixelShader;
	InputLayout _inputLayer;
};