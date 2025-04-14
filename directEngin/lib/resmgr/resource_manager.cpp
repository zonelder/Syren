#include "pch.h"

#include "resource_manager.h"
#include "xml_node.h"
#include "mesh.h"
#include "fbx_utils.h"

#include "cstdmf/file_system.h"
#include "graphic/vertex_shader.h"
#include "graphic/pixel_shader.h"

#include <fstream>
#include <unordered_set>


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
	{
		//log an error;
		return nullptr;
	}
		//res = loadFbxMeshInternal(meshes_[path], path);

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

TexturePtr ResourceManager::getTexture(const std::string& resourceID)
{
	auto path = fileSystem::simplifyPath(resourceID);
	auto it = textures_.find(resourceID);
	if (it != textures_.end())
		return it->second;

	auto ext = fileSystem::getExtension(path);
	if (!fileSystem::fileExist(path))
		return getDefaultTexture();

	if (ext != "dds" && ext != "png" && ext != "jpeg" && ext != "bmp")
	{
		return getDefaultTexture();
	}
	textures_[path] = std::make_shared<Texture>(_gfx,path);
	return textures_[path];

}

/// @brief method return the texture that engine use as default texture.as a pair of exception-free semantic in case some texture cant be loaded we return texture that can(engine should guarantie it)
/// @return 
TexturePtr ResourceManager::getDefaultTexture()
{
	return getTexture("resource/empty.dds");//TODO - make an exl config and use path from there insead of hardcode it into codebase
}

FbxPrefabPtr ResourceManager::getFbxPrefab(const std::string& resourceID)
{
	auto path = fileSystem::simplifyPath(resourceID);
	auto it = fbxPrefabs_.find(path);
	if (it != fbxPrefabs_.end())
	{
		return it->second;
	}

	if (fileSystem::getExtension(path) != "fbx")
	{
		//TODO(log) an error
		return nullptr;
	}

	fbx_utils::FbxRAII<FbxManager> manager = FbxManager::Create();
	fbx_utils::FbxRAII<FbxScene> scene = FbxScene::Create(manager, "Scene");
	fbx_utils::FbxRAII<FbxImporter> importer = FbxImporter::Create(manager, "");

	if (!importer->Initialize(resourceID.c_str(), -1, manager->GetIOSettings()))
	{
		std::cerr << "Error initializing FBX importer: " << importer->GetStatus().GetErrorString() << std::endl;
		return nullptr;
	}

	importer->Import(scene);

	fbxPrefabs_[path] = std::make_shared<FbxPrefab>(scene, manager);

	return fbxPrefabs_[path];
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
	pMesh->uvs.reserve(vertexCount);
	for (size_t i = 0; i < vertexCount; ++i)
	{
		auto& v = pMesh->vertexes.emplace_back();
		auto pos = v.position;
		if (!(fileHandler >> pos[0] >> pos[1] >> pos[2]))
		{
			return false;
		}
		auto& uv = pMesh->uvs.emplace_back();
		if (!(fileHandler >> uv.x >> uv.y))
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
		if (!(fileHandler >> color[0] >> color[1] >> color[2] >> color[3]))
		{
			return false;
		}
	}

	fileHandler.close();
	pMesh->resourceID = file;
	meshHelpers::updateBB(pMesh.get());

	return true;

}
/*
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
*/

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
		auto pos = vertex.position;
		file << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
	}

	file << pMesh->uvs.size() << std::endl;
	for (const auto& uv : pMesh->uvs)
	{
		file << uv.x << " " << uv.y << std::endl;
	}

	file << pMesh->indices.size() << std::endl;
	for (const auto& index : pMesh->indices) 
	{
		file << index << std::endl;
	}

	file << pMesh->colors.size() << std::endl;
	for (const auto& color : pMesh->colors) 
	{
		file << color[0] << " " << color[1] << " " << color[2] << " " << color[3] << std::endl;
	}

	file.close();
	return true;
}
