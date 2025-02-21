#include "fbx_utils.h"
#include "graphics/Drawable/BindComponent/vertex.h"

Color fbx_utils::toColor(FbxColor color)
{
	return { static_cast<float>(color.mRed),static_cast<float>(color.mGreen),static_cast<float>(color.mBlue),static_cast<float>(color.mAlpha) };
}

DirectX::XMFLOAT3 fbx_utils::toVector3(const FbxVector4& v)
{
	return { static_cast<float>(v[0]), static_cast<float>(v[1]), static_cast<float>(v[2])};
}

DirectX::XMFLOAT4 fbx_utils::toVector4(const FbxVector4& v)
{
	return { static_cast<float>(v[0]), static_cast<float>(v[1]), static_cast<float>(v[2]), static_cast<float>(v[3]) };
}

DirectX::XMFLOAT4 fbx_utils::toVector4(const FbxQuaternion& v)
{
	return { static_cast<float>(v[0]), static_cast<float>(v[1]), static_cast<float>(v[2]), static_cast<float>(v[3]) };
}

bool fbx_utils::isMesh(FbxNode* node)
{
	return node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh;
}

bool fbx_utils::loadMeshFromNode(FbxMesh* fbxMesh, MeshPtr mesh, const std::string& file, const FbxRAII<FbxManager>& manager)
{

	if (!fbxMesh) return false;

	// enshure fbx mesh contains only trianges as polygone
	if (!fbxMesh->IsTriangleMesh()) {
		FbxGeometryConverter converter(manager);
		auto attr = converter.Triangulate(fbxMesh, true);
		if (attr && attr->GetAttributeType() == FbxNodeAttribute::eMesh)
			fbxMesh = static_cast<FbxMesh*>(attr);
		else
			return false;
	}

	mesh->IndexCount = fbxMesh->GetPolygonCount() * 3;
	mesh->indices.reserve(mesh->IndexCount);
	mesh->vertexes.reserve(fbxMesh->GetControlPointsCount());
	mesh->colors.resize(fbxMesh->GetElementVertexColorCount());


	for (int j = 0; j < fbxMesh->GetControlPointsCount(); j++) {
		FbxVector4 vertex = fbxMesh->GetControlPointAt(j);
		Vertex v;
		v.position = DirectX::XMVectorSet(static_cast<float>(vertex[0]), static_cast<float>(vertex[1]), static_cast<float>(vertex[2]), 1.0f);
		mesh->vertexes.emplace_back(v);
	}

	for (int j = 0; j < fbxMesh->GetPolygonCount(); j++) {
		for (int k = 0; k < fbxMesh->GetPolygonSize(j); k++) {
			mesh->indices.push_back(fbxMesh->GetPolygonVertex(j, k));
		}
	}

	if (fbxMesh->GetElementVertexColorCount() > 0)
	{
		auto pColorElement = fbxMesh->GetElementVertexColor();
		for (int i = 0; i < fbxMesh->GetControlPointsCount(); ++i)
		{
			FbxColor color = pColorElement->GetDirectArray().GetAt(i);
			mesh->colors[i] = toColor(color);
		}
	}

	if (fbxMesh->GetElementUVCount() > 0)
	{

	}

	mesh->resourceID = file;
	meshHelpers::updateBB(mesh.get());
	return true;
}
