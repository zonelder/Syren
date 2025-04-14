#include "pch.h"
#include "fbx_utils.h"
#include "graphic/vertex.h"

#include <unordered_map>

Color fbx_utils::toColor(FbxColor color)
{
	return { static_cast<float>(color.mRed),static_cast<float>(color.mGreen),static_cast<float>(color.mBlue),static_cast<float>(color.mAlpha) };
}

DirectX::XMFLOAT2 fbx_utils::toVector2(const FbxVector2& v)
{
    return{ static_cast<float>(v[0]), static_cast<float>(v[1]) };
}

Vector3 fbx_utils::toVector3(const FbxVector4& v)
{
	return { static_cast<float>(v[0]), static_cast<float>(v[1]), static_cast<float>(v[2])};
}

DirectX::XMFLOAT4 fbx_utils::toVector4(const FbxVector4& v)
{
	return { static_cast<float>(v[0]), static_cast<float>(v[1]), static_cast<float>(v[2]), static_cast<float>(v[3]) };
}

Quaternion fbx_utils::toVector4(const FbxQuaternion& v)
{
	return { static_cast<float>(v[0]), static_cast<float>(v[1]), static_cast<float>(v[2]), static_cast<float>(v[3]) };
}

bool fbx_utils::isMesh(FbxNode* node)
{
	return node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh;
}

struct VertexKey
{
    int cpIndex;     
    int normalIndex;
    int uvIndex;    
    int colorIndex;

    bool operator==(const VertexKey& other) const {
        return cpIndex == other.cpIndex &&
            normalIndex == other.normalIndex &&
            uvIndex == other.uvIndex &&
            colorIndex == other.colorIndex;
    }
};

struct VertexKeyHash {
    std::size_t operator()(const VertexKey& key) const {
        std::size_t h = 0;
        std::hash<int> hasher;
        h ^= hasher(key.cpIndex) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        h ^= hasher(key.normalIndex) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        h ^= hasher(key.uvIndex) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        h ^= hasher(key.colorIndex) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        return h;
    }
};

bool fbx_utils::loadMeshFromNode(FbxMesh* fbxMesh, MeshPtr mesh, const std::string& file, const FbxRAII<FbxManager>& manager)
{

    if (!fbxMesh)
        return false;

    if (!fbxMesh->IsTriangleMesh()) {
        FbxGeometryConverter converter(manager);
        auto attr = converter.Triangulate(fbxMesh, true);
        if (attr && attr->GetAttributeType() == FbxNodeAttribute::eMesh)
            fbxMesh = static_cast<FbxMesh*>(attr);
        else
            return false;
    }


    FbxGeometryElementNormal* normalElement = fbxMesh->GetElementNormal();
    FbxGeometryElementUV* uvElement = (fbxMesh->GetElementUVCount() > 0) ? fbxMesh->GetElementUV(0) : nullptr;
    FbxGeometryElementVertexColor* colorElement = (fbxMesh->GetElementVertexColorCount() > 0) ? fbxMesh->GetElementVertexColor(0) : nullptr;

    int controlPointCount = fbxMesh->GetControlPointsCount();
    FbxVector4* controlPoints = fbxMesh->GetControlPoints();
    std::unordered_map<VertexKey, unsigned short, VertexKeyHash> vertexCache;

    int polygonVertexCounter = 0;

    int polygonCount = fbxMesh->GetPolygonCount();
    for (int polyIndex = 0; polyIndex < polygonCount; ++polyIndex) 
    {
        int polySize = fbxMesh->GetPolygonSize(polyIndex);
        for (int vertIndex = 0; vertIndex < polySize; ++vertIndex) 
        {
            int cpIndex = fbxMesh->GetPolygonVertex(polyIndex, vertIndex);

            int normalIndex = -1;
            if (normalElement) 
            {
                if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                    normalIndex = polygonVertexCounter;
                else // eByControlPoint
                    normalIndex = cpIndex;
            }

            int uvIndex = -1;
            if (uvElement) 
            {
                if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                    uvIndex = fbxMesh->GetTextureUVIndex(polyIndex, vertIndex);
                else // eByControlPoint
                    uvIndex = cpIndex;
            }


            int colorIndex = -1;
            if (colorElement) 
            {
                if (colorElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                    colorIndex = polygonVertexCounter;
                else // eByControlPoint
                    colorIndex = cpIndex;
            }


            VertexKey key{ cpIndex, normalIndex, uvIndex, colorIndex };

            auto it = vertexCache.find(key);
            if (it != vertexCache.end()) 
            {
                mesh->indices.push_back(it->second);
                continue;
            }

            FbxVector4 pos = controlPoints[cpIndex];
            Vertex vertex;
            vertex.position = DirectX::XMVectorSet(
                static_cast<float>(pos[0]),
                static_cast<float>(pos[1]),
                static_cast<float>(pos[2]),
                1.0f
            );
            mesh->vertexes.push_back(vertex);

            if (normalElement) 
            {
                FbxVector4 fbxNormal = normalElement->GetDirectArray().GetAt(normalIndex);
                mesh->normals.push_back(toVector3(fbxNormal));
            }

            if (uvElement) 
            {
                FbxVector2 fbxUV = uvElement->GetDirectArray().GetAt(uvIndex);
                fbxUV[1] = 1 - fbxUV[1];
                mesh->uvs.push_back(toVector2(fbxUV));
            }

            if (colorElement) 
            {
                FbxColor fbxColor = colorElement->GetDirectArray().GetAt(colorIndex);

                mesh->colors.push_back(toColor(fbxColor));
            }

            unsigned short newIndex = static_cast<unsigned short>(mesh->vertexes.size() - 1);
            mesh->indices.push_back(newIndex);
            vertexCache[key] = newIndex;
            ++polygonVertexCounter;
        }
    }

    mesh->IndexCount = mesh->indices.size();
    mesh->resourceID = file;
    meshHelpers::updateBB(mesh.get());
    return true;
}

