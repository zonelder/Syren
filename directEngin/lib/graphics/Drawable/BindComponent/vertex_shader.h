#pragma once
#include "IBindable.h"
#include "input_layout.h"

#include <memory>


class VertexShader : public IBindable
{
public:
	VertexShader() = default;
	VertexShader(Graphics&, const std::string& path);
	void bind(Graphics&) noexcept override;

	const std::string& resourceID() const noexcept
	{
		return _resourceID;
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> p_pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> p_pBytecodeBlob;
	InputLayout _inputLayer;
private:
	std::string _resourceID;
};

using VertexShaderPtr = std::shared_ptr<VertexShader>;
