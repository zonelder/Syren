#ifndef  __TEXTURE_H__
#define __TEXTURE_H__

#include "graphics/Graphics.h"
#include <iostream>
#include <memory>

class Texture final
{
public:

	Texture() = delete;
	Texture(Graphics& gfx,const std::string& path);

	void bind(Graphics& gfx) const;

	std::wstring getPath();

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_pTextureRV;
	Microsoft::WRL::ComPtr<ID3D11SamplerState > _pSampleState;
	std::wstring _pPath;
};


using TexturePtr = std::shared_ptr<Texture>;


#endif // ! __TEXTURE_H__