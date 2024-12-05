#pragma once
#include "../graphics/Drawable/BindComponent/vertex_shader.h"
#include "../graphics/Drawable/BindComponent/pixel_shader.h"
#include "../graphics/Drawable/BindComponent/input_layout.h"
#include "../graphics/Drawable/BindComponent/texture.h" 

#include <memory>

struct Material
{
public:

	Material() = delete;
	Material(Graphics& gfx, const wchar_t* vertexShader, const wchar_t* pixelShader);
	void bind(Graphics& gfx);

	std::wstring vertexShader;
	std::wstring pixelShader;
	Texture texture;
	DirectX::XMFLOAT4 color = { 1.0f,0.0f,0.0f,1.0f };
private:
	VertexShader _vertexShader;
	PixelShader _pixelShader;
	InputLayout _inputLayer;

};

using MaterialPtr = std::shared_ptr<Material>;
