#include "pch.h"
#include "vertex_shader.h"
#include "cstdmf/string_converter.h"

#include <stdexcept>

namespace
{
	// Вспомогательная функция для определения формата
DXGI_FORMAT determineFormat(BYTE mask, D3D_REGISTER_COMPONENT_TYPE type)
{
    if (type == D3D_REGISTER_COMPONENT_FLOAT32)
    {
        switch (mask)
        {
        case 0x1: return DXGI_FORMAT_R32_FLOAT;
        case 0x3: return DXGI_FORMAT_R32G32_FLOAT;
        case 0x7: return DXGI_FORMAT_R32G32B32_FLOAT;
        case 0xF: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
    }
    // Добавьте обработку других типов данных при необходимости
    throw std::runtime_error("determineFormat::Unsupported input element format");
}
}

VertexShader::VertexShader(Graphics& gfx, const std::string& path) : 
	_resourceID(path)
{
	INFOMAN(gfx);
	auto wPath = stringHelper::to_wstring(path);
	GFX_THROW_INFO(D3DReadFileToBlob(wPath.c_str(), &p_pBytecodeBlob));

    // Рефлексия шейдера
    Microsoft::WRL::ComPtr<ID3D11ShaderReflection> pReflector;
    GFX_THROW_INFO(D3DReflect(
        p_pBytecodeBlob->GetBufferPointer(),
        p_pBytecodeBlob->GetBufferSize(),
        IID_ID3D11ShaderReflection,
        &pReflector
    ));

	    // Получаем описание входных параметров
    D3D11_SHADER_DESC shaderDesc;
    pReflector->GetDesc(&shaderDesc);

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        pReflector->GetInputParameterDesc(i, &paramDesc);

        // Пропускаем системные значения, такие как SV_VertexID
        if (strcmp(paramDesc.SemanticName, "SV_VertexID") == 0)
            continue;

        D3D11_INPUT_ELEMENT_DESC element = {0};
        element.SemanticName = paramDesc.SemanticName;
        element.SemanticIndex = paramDesc.SemanticIndex;
        element.Format = determineFormat(paramDesc.Mask, paramDesc.ComponentType);
        element.InputSlot = i;
        element.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        element.InstanceDataStepRate = 0;
        ied.push_back(element);
    }

	GFX_THROW_INFO(gfx.getDevice()->CreateVertexShader(p_pBytecodeBlob->GetBufferPointer(), p_pBytecodeBlob->GetBufferSize(), nullptr, &p_pVertexShader));

	_inputLayer = InputLayout(gfx, ied, p_pBytecodeBlob.Get());
}


void VertexShader::bind(Graphics& gfx) noexcept
{
	gfx.getContext()->VSSetShader(p_pVertexShader.Get(), nullptr, 0u);
	_inputLayer.bind(gfx);
}