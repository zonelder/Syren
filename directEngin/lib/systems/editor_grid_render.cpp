#include "editor_grid_render.h"


static constexpr float gridSize = 5.0f;
static constexpr int gridCount = 40; // Количество линий в каждую сторону от центра
void EditorGridRenderer::onInit(SceneManager& scene)
{
    auto gfx = SceneContext::pGfx();
    INFOMAN((*gfx));
    _enttID = scene.createEntity().getID();
    scene.addComponent<Transform>(_enttID);
    auto& r = scene.addComponent<Render>(_enttID);
    r.pMesh = std::make_shared<Mesh>();

    auto& vertices = r.pMesh->vertexes;

    for (int i = -gridCount; i <= gridCount; ++i)
    {
        float offset = i * gridSize;
        vertices.push_back({ { offset, 0.0f, -gridSize * gridCount } });
        vertices.push_back({ { offset, 0.0f,  gridSize * gridCount } });

        vertices.push_back({ { -gridSize * gridCount, 0.0f, offset } });
        vertices.push_back({ {  gridSize * gridCount, 0.0f, offset } });
    }

    r.pMaterial = std::make_shared<Material>(*gfx);
    r.pMaterial->pVertexShader = SceneContext::pResources()->getVertexShader("shaders/GridVertex.cso");
    r.pMaterial->pPixelShader = SceneContext::pResources()->getPixelShader("shaders/GridPixel.cso");
    r.pMaterial->color = { 0.25f, 0.25f, 0.25f, 1.0f };
    r.topology = Topology(*gfx, D3D_PRIMITIVE_TOPOLOGY_LINELIST);

}

void EditorGridRenderer::onUpdate(SceneManager& scene, float dTime)
{
    auto& cam = scene.getCamera();
    const auto& camPosition = cam.transform.position;
    auto& tr = scene.getComponent<Transform>(_enttID);
    tr.position.x = std::floor(camPosition.x / gridSize) * gridSize;
    tr.position.z = std::floor(camPosition.z / gridSize) * gridSize;
    tr.position.y = 0.0f;
}

