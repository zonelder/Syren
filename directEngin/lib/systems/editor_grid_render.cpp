#include "editor_grid_render.h"


static constexpr float gridSize = 5.0f;
static constexpr int gridCount = 40; // Количество линий в каждую сторону от центра
void EditorGridRenderer::onInit(SceneManager& scene)
{
    auto gfx = SceneContext::pGfx();
    INFOMAN((*gfx));
    _vertexConstantBuffer = VertexConstantBuffer<DirectX::XMMATRIX>(*gfx, _wvp);
    D3D11_BUFFER_DESC constantBufferDesc = {};

    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    constantBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.MiscFlags = 0;
    GFX_THROW_INFO(gfx->getDevice()->CreateBuffer(&constantBufferDesc, nullptr, &p_colorConstantBuffer));


    // Округляем положение камеры до ближайшего узла сетки
    float gridOffsetX = 0.0f;
    float gridOffsetZ = 0.0f;

    for (int i = -gridCount; i <= gridCount; ++i)
    {
        float offset = i * gridSize;
        // Вертикальные линии
        vertices_.push_back({ { gridOffsetX + offset, 0.0f, gridOffsetZ - gridSize * gridCount } });
        vertices_.push_back({ { gridOffsetX + offset, 0.0f, gridOffsetZ + gridSize * gridCount } });

        // Горизонтальные линии
        vertices_.push_back({ { gridOffsetX - gridSize * gridCount, 0.0f, gridOffsetZ + offset } });
        vertices_.push_back({ { gridOffsetX + gridSize * gridCount, 0.0f, gridOffsetZ + offset } });
    }

    _vertexBuffer = VertexBuffer(*gfx, vertices_);
    _pMaterial = std::make_shared<Material>(*gfx);
    _pMaterial->pVertexShader = SceneContext::pResources()->getVertexShader("shaders/GridVertex.cso");
    _pMaterial->pPixelShader = SceneContext::pResources()->getPixelShader("shaders/GridPixel.cso");
    _pMaterial->color = { 0.25f, 0.25f, 0.25f, 1.0f };
    _topology = Topology(*gfx, D3D_PRIMITIVE_TOPOLOGY_LINELIST);

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

    _pMaterial->bind(gfx);
    _topology.bind(gfx);

    context->UpdateSubresource(p_colorConstantBuffer.Get(), 0, nullptr, &(_pMaterial->color), sizeof(DirectX::XMFLOAT4), 0);
    context->PSSetConstantBuffers(1u, 1u, p_colorConstantBuffer.GetAddressOf());

    context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

    gfx.Draw(vertices_.size(),0);
	
}
