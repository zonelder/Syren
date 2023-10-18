#pragma once
#include "../graphics/Drawable/BindComponent/VertexShader.h"
#include "../graphics/Drawable/BindComponent/PixelShader.h"
#include "../graphics/Drawable/BindComponent/InputLayout.h"
struct Material
{
public:

	Material() = delete;
	Material(Graphics& gfx, const wchar_t* vertexShader, const wchar_t* pixelShader);
	void bind(Graphics& gfx);

	std::wstring vertexShader;
	std::wstring pixelShader;
private:



	VertexShader _vertexShader;
	PixelShader _pixelShader;
	InputLayout _inputLayer;
};