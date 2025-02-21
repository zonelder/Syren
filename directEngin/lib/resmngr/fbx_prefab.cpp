#include "fbx_prefab.h"
#include "fbx_utils.h"
#include <iostream>

struct alignas(16) Transform
{
    DirectX::XMMATRIX localMatrix;

    Transform() : localMatrix(DirectX::XMMatrixIdentity()) {}

    explicit Transform(const FbxAMatrix& fbxMatrix)
    {
        float m[4][4];
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = static_cast<float>(fbxMatrix.Get(i, j));

        localMatrix = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(m));
    }
};


FbxPrefab::FbxPrefab(const fbx_utils::FbxRAII<FbxScene>& scene,const fbx_utils::FbxRAII<FbxManager>& manager)
{

    FbxNode* rootNode = scene->GetRootNode();
    if (!rootNode)
        return;
    // Очистка контейнеров перед загрузкой нового префаба
    meshes_.clear();
    nodes_.clear();
    
    meshCache meshCache;

    // Обрабатываем всю сцену начиная с корня
    for (int i = 0; i < rootNode->GetChildCount(); ++i)
    {
        processNode(rootNode->GetChild(i), -1, manager, meshCache);
    }
    return;
}
void FbxPrefab::processNode(FbxNode* fbxNode, int parentIndex, const fbx_utils::FbxRAII<FbxManager>& manager,meshCache& cache)
{
    if (!fbxNode)
        return;

    // Создаём запись для текущего узла.
    Node node;
    node.name = fbxNode->GetName();
    node.parent = (parentIndex < 0) ? -1 :parentIndex;
    node.meshID = -1;
    node.materialID = -1;
    node.vertexShaderID = -1;
    node.pixelShaderID = -1;

    node.meshID = processMesh(fbxNode->GetMesh(),manager,cache);
    // Use FbxNode methods to get local transformation data
    FbxAMatrix localTransform = fbxNode->EvaluateLocalTransform();

    node.position = fbx_utils::toVector3(localTransform.GetT());
    node.rotation = fbx_utils::toVector4(localTransform.GetQ());
    node.scale = fbx_utils::toVector3(localTransform.GetS());

    int currentIndex = static_cast<int>(nodes_.size());
    nodes_.push_back(node);

    for (int i = 0; i < fbxNode->GetChildCount(); ++i)
    {
        processNode(fbxNode->GetChild(i), currentIndex, manager, cache);
    }
}

short FbxPrefab::processMesh(FbxMesh* fbxMesh, const fbx_utils::FbxRAII<FbxManager>& manager, meshCache& cache)
{
    if (!fbxMesh)
        return -1;
  
    // Проверяем, загружен ли уже этот меш
    auto it = cache.find(fbxMesh);
    if (it != cache.end())
    {
        // Если меш уже загружен, просто используем его индекс
        return it->second;
    }

    auto mesh = std::make_shared<Mesh>();
    std::string file = fbxMesh->GetName();
    if (!fbx_utils::loadMeshFromNode(fbxMesh, mesh, file, manager))
    {
        std::cerr << "Failed to load mesh for node: " << file << std::endl;
        return -1;
    }
    
    short meshIndex = static_cast<short>(meshes_.size());
    meshes_.push_back(mesh);
    cache[fbxMesh] = meshIndex;
    return meshIndex;
}
