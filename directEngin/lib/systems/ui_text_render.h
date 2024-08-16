#pragma once
#include "../common/Scene/BaseSystem.h"
#include <memory>

#include <initguid.h>
#include "SpriteFont.h"
#include "SpriteBatch.h"

using namespace DirectX::DX11;

class UITextRender: public BaseSystem
{
public:

	/// @brief Create new instance of UITextRender system
	/// @param gfx Graphic class where render goes
	/// @param font name of .spritefont
	UITextRender(Graphics& gfx,const wchar_t* font);
	UITextRender() = delete;

	void onFrame(SceneManager&) override;

private:
	std::unique_ptr<SpriteBatch> p_spriteBatch;
	std::unique_ptr<SpriteFont> p_spriteFont;

};

