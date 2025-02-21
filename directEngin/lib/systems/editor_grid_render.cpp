#include "editor_grid_render.h"


static constexpr float gridSize = 5.0f;
static constexpr int gridCount = 40; // Количество линий в каждую сторону от центра
void EditorGridRenderer::onInit(SceneManager& scene)
{
    auto gfx = SceneContext::pGfx();
    INFOMAN((*gfx));
    _vertexConstantBuffer = VertexConstantBuffer<DirectX::XMMATRIX>(*gfx, _wvp);


    // Округляем положение камеры до ближайшего узла сетки
    float gridOffsetX = 0.0f;// std::floor(camPosition.x / gridSize)* gridSize;
    float gridOffsetZ = 0.0f;//std::floor(camPosition.z / gridSize) * gridSize;

    static DirectX::XMVECTOR color = { 0.25f, 0.25f, 0.25f, 1.0f };
    for (int i = -gridCount; i <= gridCount; ++i)
    {
        float offset = i * gridSize;
        // Вертикальные линии
        vertices_.push_back({ { gridOffsetX + offset, 0.0f, gridOffsetZ - gridSize * gridCount }, color });
        vertices_.push_back({ { gridOffsetX + offset, 0.0f, gridOffsetZ + gridSize * gridCount }, color });

        // Горизонтальные линии
        vertices_.push_back({ { gridOffsetX - gridSize * gridCount, 0.0f, gridOffsetZ + offset }, color });
        vertices_.push_back({ { gridOffsetX + gridSize * gridCount, 0.0f, gridOffsetZ + offset }, color });
    }

    _vertexBuffer = VertexBuffer(*gfx, vertices_);

    //create vertex shader(almost full copy from vertexShader.cpp
    std::wstring wPath = L"shaders/GridVertex.cso"; // stringHelper::to_wstring(path);
    GFX_THROW_INFO(D3DReadFileToBlob(wPath.c_str(), &p_pBytecodeBlob));
    GFX_THROW_INFO(gfx->getDevice()->CreateVertexShader(p_pBytecodeBlob->GetBufferPointer(), p_pBytecodeBlob->GetBufferSize(), nullptr, &p_pVertexShader));

    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = 
    {
        {"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},//DXGI_FORMAT_R32G32B32_FLOAT
        {"Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    _inputLayer = InputLayout(*gfx, ied, p_pBytecodeBlob.Get());

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    wPath = L"shaders/GridPixel.cso";
    GFX_THROW_INFO(D3DReadFileToBlob(wPath.c_str(), &pBlob));
    GFX_THROW_INFO(gfx->getDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &p_pPixelShader));

}

void EditorGridRenderer::onFrame(SceneManager& scene)
{
    Graphics& gfx = *SceneContext::pGfx();
    auto* context = gfx.getContext();
    INFOMAN(gfx);
	auto& cam = scene.getCamera();
	auto viewProjection = cam.view() * cam.projection();
	const auto& camPosition = cam.transform.position;
    // Округляем положение камеры до ближайшего узла сетки
    float gridOffsetX = std::floor(camPosition.x / gridSize)* gridSize;
    float gridOffsetZ = std::floor(camPosition.z / gridSize) * gridSize;
    auto world = DirectX::XMMatrixTranslation(gridOffsetX,0, gridOffsetZ);
    _wvp = DirectX::XMMatrixTranspose(world * viewProjection);


    auto pConstantBuffer = _vertexConstantBuffer.p_pConstantBuffer;
    D3D11_MAPPED_SUBRESOURCE msr;
    GFX_THROW_INFO(context->Map(
        pConstantBuffer.Get(), 0u,
        D3D11_MAP_WRITE_DISCARD, 0u,
        &msr
    ));
    memcpy(msr.pData, &_wvp, sizeof(_wvp));
    context->Unmap(pConstantBuffer.Get(), 0u);

    _vertexConstantBuffer.bind(gfx);
    _vertexBuffer.bind(gfx);


    gfx.getContext()->PSSetShader(p_pPixelShader.Get(), nullptr, 0u);
    gfx.getContext()->VSSetShader(p_pVertexShader.Get(), nullptr, 0u);
    _inputLayer.bind(gfx);

    context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    context->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

    gfx.Draw(vertices_.size(),0);
	
}
