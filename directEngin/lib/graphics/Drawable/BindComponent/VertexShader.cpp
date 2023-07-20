#include "VertexShader.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
	INFOMAN(gfx);
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &p_pBytecodeBlob));
	GFX_THROW_INFO(gfx.getDevice()->CreateVertexShader(p_pBytecodeBlob->GetBufferPointer(), p_pBytecodeBlob->GetBufferSize(), nullptr, &p_pVertexShader));
}


void VertexShader::bind(Graphics& gfx) noexcept
{
	gfx.getContext()->VSSetShader(p_pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::getBytecode() const noexcept
{
	return p_pBytecodeBlob.Get();
}