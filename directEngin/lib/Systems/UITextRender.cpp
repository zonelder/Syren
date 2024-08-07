#include "UITextRender.h"
#include "../component/TextUI.h"
#include <iostream>

UITextRender::UITextRender(Graphics& gfx, const wchar_t* font)
{	
	p_spriteBatch = std::make_unique<SpriteBatch>(gfx.getContext());
	p_spriteFont = std::make_unique<SpriteFont>(gfx.getDevice(), font);
}



void UITextRender::onFrame(SceneManager& scene)
{

	p_spriteBatch->Begin();
	// ��������� ���� � �������������� range-based for ����� ����� getEntityWith<TextUI> � range-based
	for (auto& entt : scene.getEntitiesWith<TextUI>())
	{
		auto& textUI = scene.getComponent<TextUI>(entt);
		p_spriteFont->DrawString(p_spriteBatch.get(), textUI.content.c_str(), textUI.position, DirectX::Colors::Black, 0.f);
	}
	p_spriteBatch->End();

}

