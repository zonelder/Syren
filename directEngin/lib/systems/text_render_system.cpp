#include "text_render_system.h"
#include "components/text.h"




TextRenderSystem::TextRenderSystem(Graphics& gfx)
{
	IFW1Factory* pFW1Factory;
	auto HR = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
	if (FAILED(HR))
	{
		MessageBox(NULL, TEXT("FW1CreateFactory"), TEXT("Error"), MB_OK);
		return;
	}

	// Set up the font-wrapper to use mip-mapping and anisotropic filtering for better visuals with transformed text
	FW1_FONTWRAPPERCREATEPARAMS createParams = { 0 };
	createParams.SheetMipLevels = 5;
	createParams.AnisotropicFiltering = TRUE;
	createParams.DefaultFontParams.pszFontFamily = L"Arial";
	createParams.DefaultFontParams.FontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	createParams.DefaultFontParams.FontStyle = DWRITE_FONT_STYLE_NORMAL;
	createParams.DefaultFontParams.FontStretch = DWRITE_FONT_STRETCH_NORMAL;


	HR = pFW1Factory->CreateFontWrapper(gfx.getDevice(), NULL, &createParams, &p_fontWrapper);

	if (FAILED(HR))
	{
		MessageBox(NULL, TEXT("CreateFontWrapper"), TEXT("Error"), MB_OK);
		return;
	}
	pFW1Factory->Release();


}


void TextRenderSystem::onFrame(SceneManager& scene)
{
	Graphics& gfx = *SceneContext::pGfx();
	auto& _texts = scene.getPool<Text>();
	auto& _transforms = scene.getPool<Transform>();

	for (auto [entt,text,tr] : scene.view<Text, Transform>())
	{
		FW1_RECTF rect = { 0.0f, 1.0f, 1.0f, 0.0f };
		p_fontWrapper->DrawString(
			gfx.getContext(),
			text.content.c_str(),
			L"Arial",
			100,
			10,
			10,
			0xff0000ff,
			FW1_CENTER | FW1_VCENTER | FW1_STATEPREPARED | FW1_BUFFERSPREPARED
		);
	
	}
}


TextRenderSystem::~TextRenderSystem()
{
	p_fontWrapper->Release();
}