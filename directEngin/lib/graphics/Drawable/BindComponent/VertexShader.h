#pragma once
#include "IBindable.h"

class VertexShader : public IBindable
{
public:
	VertexShader() = default;
	VertexShader(Graphics&, const std::wstring& path);
	void bind(Graphics&) noexcept override;
	ID3DBlob* getBytecode() const noexcept;

protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> p_pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> p_pBytecodeBlob;
};
