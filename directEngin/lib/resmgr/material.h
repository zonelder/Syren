#pragma once

#include <memory>

#include "math/color.h"
#include "graphic/vertex_shader.h"
#include "graphic/pixel_shader.h"
#include "graphic/texture.h" 

struct Material
{
public:

	Material() = delete;
	Material(Graphics& gfx);
	void bind(Graphics& gfx);

	TexturePtr pTexture{ nullptr };
	Color color = Color::red;
	VertexShaderPtr pVertexShader{ nullptr };
	PixelShaderPtr pPixelShader{ nullptr };
	std::string resourceID;
};

using MaterialPtr = std::shared_ptr<Material>;
