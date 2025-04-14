#ifndef __SCENE_CONTEXT_H__
#define __SCENE_CONTEXT_H__
#include "core/window.h"
#include "resmngr/resource_manager.h"
#include "core/input.h"


/// @brief Some usefull context of app.its support overriding of context but  its always should be at least one instance of class.
/// in commpon case class App provide such an instance so be carefull when you overriding mainContext;
class SceneContext
{
public:
	/// @brief create new instance of scene context.its forbidden to pass nullptr to this constructor.
	///cause contest SHALL BE SERTAN AND WELL DEFINED.
	SceneContext(const Window& wnd);
	~SceneContext();


	/// @brief get main resource manager of application
	/// @return 
	static ResourceManager& resources() noexcept { return s_pMainContext->_pResourceManager; }

	/// @brief get main graphic handler of application
	/// @return 
	static Graphics* pGfx() noexcept { return &(s_pMainContext->_pGraphics); }

	static Input& input() noexcept { return (s_pMainContext)->_input; }

	static void setMainContext(SceneContext* context)
	{
		assert(context != nullptr);
		s_pMainContext = context;
	}

	void updateInput(const Window& wnd);

private:
	ResourceManager _pResourceManager;
	Graphics _pGraphics;
	Input _input;

	static std::vector<SceneContext*> s_contexts;
	static SceneContext* s_pMainContext;

};

/// @brief shortcut to commonly use features
namespace context
{
	ResourceManager& rm();
	Graphics& gfx();
	Input& input();

	TexturePtr		getTexture(const std::string& path);
	MeshPtr			getMesh(const std::string& path);
	MaterialPtr		getMaterial(const std::string& path);
	VertexShaderPtr getVertexShader(const std::string& path);
	PixelShaderPtr  getPixelShader(const std::string& path);

}


#endif