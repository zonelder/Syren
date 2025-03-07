#ifndef __GEOMETRIC_BUFFER_H__
#define __GEOMETRIC_BUFFER_H_

#include "render_target.h"
#include <memory>

class GBuffer
{
public:
	GBuffer(Graphics* pGfx);

	void bind(Graphics* pGfx);
	void unbind(Graphics* pGfx);

	void clear(Graphics* pGfx, const float color[4]);

	auto getAlbedo() const noexcept
	{
		return _pAlbedo.get();
	}

	auto getNormal() const noexcept
	{
		return _pNormal.get();
	}

	auto getDepth() const noexcept
	{
		return _pDepth.get();
	}

private:
	std::unique_ptr<RenderTarget> _pAlbedo;
	std::unique_ptr<RenderTarget> _pNormal;
	std::unique_ptr<RenderTarget> _pDepth;
};


#endif