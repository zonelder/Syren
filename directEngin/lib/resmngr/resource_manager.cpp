
#include "resmngr/resource_manager.h"
#include "resmngr/xml_node.h"
#include "components/mesh.h"
#include "graphics/Drawable/BindComponent/vertex_shader.h"
#include "graphics/Drawable/BindComponent/pixel_shader.h"
#include <filesystem>
#include <fstream>
#include <unordered_set>

#include <fbxsdk.h>


template<class T>
class FbxRAII
{
public:
	FbxRAII() : _pObj(nullptr) {};

	FbxRAII(T* pObj) : _pObj(pObj) {};

	~FbxRAII()
	{
		if (_pObj)
		{
			_pObj->Destroy();
			_pObj = nullptr;
		}
	}


	T& operator*() const
	{
		return *_pObj;
	}

	T* operator->() const
	{
		return _pObj;
	}

	operator T* () const noexcept { return _pObj; }

	T* get() const noexcept { return _pObj; }



private:
	T* _pObj;
};



namespace fileSystem
{
	/// @brief get extension of file
	/// @param file name. or full file name
	/// @return 
	std::string getExtension(const std::string& file)
	{
		size_t lastDot = file.find_last_of('.', file.length());
		if (lastDot != std::string::npos)
		{
			return file.substr(lastDot + 1);
		}
		return {};
	}

	/// @brief get local file name without extension
	/// @param file some file name.
	/// @return 
	std::string getFilename(const std::string& file)
	{
		size_t lastSlash = file.find_last_of("/\\");
		size_t lastDot = file.find_last_of('.', file.length());
		if (lastSlash == std::string::npos)
		{
			return file;
		}

		return file.substr(lastSlash + 1, lastDot);
	}

	bool fileExist(const std::string& file)
	{
		return std::filesystem::exists(file);
	}

	/// @brief get fullpath from relative path
	/// @param path #relative or fullpath to object;
	/// @return 
	std::string resolvePath(const std::string& path)
	{
		if(path.empty())
			return "";

		std::error_code ec;
		std::filesystem::path fsPath(path);
		auto absolutePath = std::filesystem::absolute(fsPath, ec);

		if(ec)
		{
			//TODO(log) error to resolve path
			return "";
		}

		return absolutePath.string();
	}

	std::string relativePath(const std::string& path)
	{
		std::error_code ec;
		std::filesystem::path fsPath(path);
		auto relativePath = std::filesystem::relative(fsPath, std::filesystem::current_path(), ec);
		if(ec)
		{
			//TODO(log) error to resolve path
			return "";
		}
		return relativePath.string();
	}

	std::wstring relativePath(const std::wstring& path)
	{
		std::error_code ec;
		std::filesystem::path fsPath(path);
		auto relativePath = std::filesystem::relative(fsPath, std::filesystem::current_path(), ec);
		if(ec)
		{
			//TODO(log) error to resolve path
			return L"";
		}
		return relativePath.wstring();
	}

	std::wstring resolvePath(const std::wstring& path)
	{
		std::error_code ec;
		std::filesystem::path fsPath(path);
		auto absolutePath = std::filesystem::absolute(fsPath, ec);
		if(ec)
		{
			//TODO(log) error to resolve path
			return L"";
		}
		return absolutePath.wstring();
	}

	std::string simplifyPath(const std::string& path)
	{
		auto path1 = fileSystem::resolvePath(path);
		return fileSystem::relativePath(path1);
	}

	std::wstring simplifyPath(const std::wstring& path)
	{
		auto path1 = fileSystem::resolvePath(path);
		return fileSystem::relativePath(path1);
	}


}


namespace
{
	Color toColor(FbxColor color)
	{
		return { static_cast<float>(color.mRed),static_cast<float>(color.mGreen),static_cast<float>(color.mBlue),static_cast<float>(color.mAlpha) };
	}

	bool loadMeshFromNode(FbxNode* node, MeshPtr mesh, const std::string& file,const FbxRAII<FbxManager>& manager)
	{
		if (!node->GetNodeAttribute() || node->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh)
			return false;

		FbxMesh* fbxMesh = node->GetMesh();
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

		mesh->IndexCount = fbxMesh->GetPolygonCount()*3;
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

		mesh->resourceID = file;
		meshHelpers::updateBB(mesh.get());
		return true;
	}
}

/// @brief //////////////////////RESOURCE MANAGER ////////////////////

ResourceManager::ResourceManager(Graphics& gfx) : _gfx(gfx)
{
}


/// @brief getPtr to exact resource.if resource isnot loaded then load it and return to user;
/// @param resourceID full path to resource;
/// @return 
MeshPtr ResourceManager::getMesh(const std::string& resourceID)
{
	auto path = fileSystem::simplifyPath(resourceID);
	auto it = meshes_.find(path);
	if (it != meshes_.end())
	{
		return (it->second);
	}

	auto ext = fileSystem::getExtension(path);
	if (ext != "syrenmesh" && ext != "fbx")
	{
		//TODO(log) an error
		return nullptr;
	}
	meshes_[path] = std::make_shared<Mesh>();

	
	bool res = false;
	if (ext == "syrenmesh")
		res = loadSyrenMeshInternal(meshes_[path], path);
	else if (ext == "fbx")
		res = loadFbxMeshInternal(meshes_[path], path);

	if (!res)
	{
		meshes_.erase(path);
		return nullptr;
	}
	return meshes_[path];
}

VertexShaderPtr ResourceManager::getVertexShader(const std::string& resourceID)
{
	auto path = fileSystem::simplifyPath(resourceID);
	auto it = vertexShaders_.find(path);
	if (it != vertexShaders_.end())
	{
		return it->second;
	}

	if (fileSystem::fileExist(path) && fileSystem::getExtension(path) == "cso")
	{
		auto relPath = fileSystem::relativePath(path);
		vertexShaders_[path] = std::make_shared<VertexShader>(_gfx, relPath);
		return vertexShaders_[path];
	}

	return nullptr;
}

PixelShaderPtr ResourceManager::getPixelShader(const std::string& resourceID)
{
	auto path = fileSystem::simplifyPath(resourceID);
	auto it = pixelShaders_.find(path);
	if (it != pixelShaders_.end())
	{
		return it->second;
	}

	if (fileSystem::fileExist(path) && fileSystem::getExtension(path) == "cso")
	{
		auto relPath = fileSystem::relativePath(path);
		pixelShaders_[path] = std::make_shared<PixelShader>(_gfx, relPath);
		return pixelShaders_[path];
	}

	return nullptr;
}

MaterialPtr ResourceManager::getMaterial(const std::string& resourceID)
{
	auto path = fileSystem::simplifyPath(resourceID);
	auto it = materials_.find(path);
	if (it != materials_.end())
		return it->second;

	if (fileSystem::getExtension(path) != "syrenmaterial")
	{
		//TODO(log) an error
		return nullptr;
	}

	materials_[path] = std::make_shared<Material>(_gfx);

	auto res = loadMaterialInternal(materials_[path], path);
	if (!res)
	{
		materials_.erase(path);
		return nullptr;
	}

;	return materials_[path];
}

bool ResourceManager::saveMesh(const MeshPtr pMesh, const std::string& resourceID)
{
	if (fileSystem::getExtension(resourceID) != "syrenmesh")
	{
		//TODO(log) an error
		return false;
	}
	//TODO check if this pMesh is unhandled pMesh and make it handeled;
	return saveMeshInternal(pMesh, resourceID);
}

bool ResourceManager::saveMaterial(const MaterialPtr pMaterial, const std::string& resourceID)
{
	if(!pMaterial)
		return false;

	std::ofstream file(resourceID);
	if (!file.is_open())
	{
		return false;
	}

	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("Material");
	XMLNode rootNode(root);

	auto colorNode = rootNode.saveGetChild("Color");
	colorNode.setValue(pMaterial->color);
	
	if(pMaterial->pVertexShader)
	{
		auto vertexShaderNode = rootNode.saveGetChild("VertexShader");
		vertexShaderNode.setValue(fileSystem::relativePath(pMaterial->pVertexShader->resourceID()));
	}

	if(pMaterial->pPixelShader)
	{
		auto pixelShaderNode = rootNode.saveGetChild("PixelShader");
		pixelShaderNode.setValue(fileSystem::relativePath( pMaterial->pPixelShader->resourceID()));
	}

	//TODO textures can be uuser defined in runtime and not loaded from file. have to defer this
	if(pMaterial->pTexture && pMaterial->pTexture->getPath().empty())
	{
		auto textureNode = rootNode.saveGetChild("Texture");
		textureNode.setValue(fileSystem::relativePath( pMaterial->pTexture->getPath()));
	}

	doc.save(file, "\t", pugi::format_default);
	file.close();
	//todo save as xml 

	return true;
}

bool ResourceManager::loadSyrenMeshInternal(MeshPtr pMesh,const std::string& file)
{
	if(!fileSystem::fileExist(file))
		return false;

	std::ifstream fileHandler(file);
	if (!fileHandler.is_open()) {
		//std::cerr << "�� ������� ������� ����: " << filename << std::endl;
		return false;
	}
	std::string line;
	size_t vertexCount = 0u;
	size_t indexCount = 0u;
	size_t colorCount =  0u;

	if (!(fileHandler >> vertexCount))
	{
		return false;
	}
	pMesh->vertexes.reserve(vertexCount);
	for (size_t i = 0; i < vertexCount; ++i)
	{
		auto& v = pMesh->vertexes.emplace_back();
		auto pos = v.position.m128_f32;
		if (!(fileHandler >> pos[0] >> pos[1] >> pos[2]))
		{
			return false;
		}

		if (!(fileHandler >> v.uv.x >> v.uv.y))
		{
			return false;
		}
	}


	if (!(fileHandler >> indexCount))
	{
		return false;
	}

	pMesh->indices.reserve(indexCount);

	for (size_t i = 0; i < indexCount; ++i)
	{
		auto& index = pMesh->indices.emplace_back();
		if (!(fileHandler >> index))
		{
			return false;
		}
	}

	if (!(fileHandler >> colorCount))
	{
		return false;
	}

	pMesh->colors.reserve(colorCount);

	for (size_t i = 0; i < colorCount; ++i)
	{
		auto& color = pMesh->colors.emplace_back();
		if (!(fileHandler >> color.r >> color.g >> color.b >> color.a))
		{
			return false;
		}
	}

	fileHandler.close();
	pMesh->resourceID = file;
	meshHelpers::updateBB(pMesh.get());

	return true;

}

bool ResourceManager::loadFbxMeshInternal(MeshPtr mesh, const std::string& file)
{
	FbxRAII<FbxManager> manager = FbxManager::Create();
	FbxRAII<FbxScene> scene = FbxScene::Create(manager, "Scene");
	FbxRAII<FbxImporter> importer = FbxImporter::Create(manager, "");

    if (!importer->Initialize(file.c_str(), -1, manager->GetIOSettings())) 
	{
        std::cerr << "Error initializing FBX importer: " << importer->GetStatus().GetErrorString() << std::endl;
        return false;
    }

    importer->Import(scene);

    FbxNode* rootNode = scene->GetRootNode();
	if (!rootNode)
		return true;

    for (int i = 0; i < rootNode->GetChildCount(); ++i)
	{
        if (loadMeshFromNode(rootNode->GetChild(i), mesh, file,manager)) 
		{
            break; // Load only the first mesh found
        }
    }
    return true;
}


bool ResourceManager::loadMaterialInternal(MaterialPtr pMat, const std::string& file)
{
	if (!fileSystem::fileExist(file) || !pMat || fileSystem::getExtension(file) != "syrenmaterial")
		return false;

	pugi::xml_document doc;
	auto res = doc.load_file(file.c_str());
	if(!res)
	{
		std::cerr << "Parsing Error:: " << res.description() << std::endl;
		return false;
	}


	XMLNode rootNode(doc.root().first_child());
	auto colorNode = rootNode.child("Color");
	pMat->resourceID = file;
	pMat->color = colorNode.value(pMat->color);
	auto textureNode = rootNode.child("Texture");
	if(textureNode)
	{
		auto texturePath = textureNode.value<std::wstring>();
		if(!texturePath.empty())
		{
			pMat->pTexture = std::make_shared<Texture>(_gfx, texturePath.c_str());
		}
	}
	
	auto vertexShaderNode = rootNode.child("VertexShader");
	if(vertexShaderNode)
	{
		pMat->pVertexShader = getVertexShader(vertexShaderNode.value<std::string>());
	}

	auto pixelShaderNode = rootNode.child("PixelShader");
	if(pixelShaderNode)
	{
		pMat->pPixelShader = getPixelShader(pixelShaderNode.value<std::string>());
	}

	return true;
}

bool ResourceManager::saveMeshInternal(const MeshPtr pMesh, const std::string& filename)
{
	std::ofstream file(filename);
	if (!file.is_open()) 
	{
		return false;
	}

	file << pMesh->vertexes.size() << std::endl;
	for (const auto& vertex : pMesh->vertexes) 
	{
		auto pos = vertex.position.m128_f32;
		file << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
		file << vertex.uv.x << " " << vertex.uv.y << std::endl;
	}

	file << pMesh->indices.size() << std::endl;
	for (const auto& index : pMesh->indices) 
	{
		file << index << std::endl;
	}

	file << pMesh->colors.size() << std::endl;
	for (const auto& color : pMesh->colors) 
	{
		file << color.r << " " << color.g << " " << color.b << " " << color.a << std::endl;
	}

	file.close();
	return true;
}
