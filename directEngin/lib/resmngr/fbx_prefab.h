#ifndef __FBX_PREFAB__
#define __FBX_PREFAB__
#include <vector>
#include <string>
#include "fbx_utils.h"
#include <memory>
#include <unordered_map>

// Предположим, что ResourceContainer_t определён как контейнер для умных указателей.
template<typename T>
using ResourceContainer_t = std::vector<std::shared_ptr<T>>;

struct Mesh;
class VertexShader;
class PixelShader;
struct Material;

using meshCache = std::unordered_map< FbxMesh*, short>;



class FbxPrefab
{
public:

    FbxPrefab(const fbx_utils::FbxRAII<FbxScene>& scene, const fbx_utils::FbxRAII<FbxManager>& manager);

    struct Node
    {
        std::string name;
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 rotation;
        DirectX::XMFLOAT3 scale;
        int parent;
        short meshID;
        short materialID;
        short vertexShaderID;
        short pixelShaderID;

    };

    const std::vector<Node>& getNodes() const noexcept
    {
        return nodes_;
    }

    const MeshPtr getMesh(short index) const
    {
        return meshes_[index];
    }

private:

    void processNode(FbxNode* fbxNode, int parentIndex, const fbx_utils::FbxRAII<FbxManager>& manager, meshCache& cache);
    short processMesh(FbxMesh* fbxNode, const fbx_utils::FbxRAII<FbxManager>& manager, meshCache& cache);

    // Контейнеры ресурсов, полученных из FBX-файла.
    ResourceContainer_t<Mesh>           meshes_;
    ResourceContainer_t<VertexShader>   vertexShaders_;
    ResourceContainer_t<PixelShader>    pixelShaders_;
    ResourceContainer_t<Material>       materials_;
    std::vector<Node>                   nodes_;
};


#endif