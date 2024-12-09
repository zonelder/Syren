#include "mesh_pool.h"

#include <Windows.h>

#include "resmngr/xml_node.h"
#include "components/mesh.h"
#include "components/material.h"
#include "graphics/Drawable/BindComponent/vertex_shader.h"
#include "graphics/Drawable/BindComponent/pixel_shader.h"
#include <filesystem>
#include <fstream>

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
		//char fullPath[MAX_PATH];
		std::string fullPath;
		fullPath.reserve(MAX_PATH);
		
		if (GetFullPathName(path.c_str(), MAX_PATH, &fullPath[0], nullptr) == 0)
		{
			//TODO(log) error to resolve path
			return "";
		}

		return fullPath;
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
	auto path = fileSystem::resolvePath(resourceID);
	auto it = meshes_.find(path);
	if (it != meshes_.end())
	{
		return (it->second);
	}

	if (fileSystem::getExtension(path) != "syrenmesh")
	{
		//TODO(log) an error
		return nullptr;
	}
	meshes_[path] = std::make_shared<Mesh>();

	auto res = loadMeshInternal(meshes_[path], path);
	if (!res)
	{
		meshes_.erase(path);
		return nullptr;
	}
	return meshes_[path];
}

VertexShaderPtr ResourceManager::getVertexShader(const std::string& resourceID)
{
	auto path = fileSystem::resolvePath(resourceID);
	auto it = vertexShaders_.find(path);
	if (it != vertexShaders_.end())
	{
		return it->second;
	}

	if (fileSystem::fileExist(path) && fileSystem::getExtension(path) == "cso")
	{
		vertexShaders_[path] = std::make_shared<VertexShader>(_gfx, path);
		return vertexShaders_[path];
	}

	return nullptr;
}

PixelShaderPtr ResourceManager::getPixelShader(const std::string& resourceID)
{
	auto path = fileSystem::resolvePath(resourceID);
	auto it = pixelShaders_.find(path);
	if (it != pixelShaders_.end())
	{
		return it->second;
	}

	if (fileSystem::fileExist(path) && fileSystem::getExtension(path) == "cso")
	{
		pixelShaders_[path] = std::make_shared<PixelShader>(_gfx, path);
		return pixelShaders_[path];
	}

	return nullptr;
}

MaterialPtr ResourceManager::getMaterial(const std::string& resourceID)
{
	auto path = fileSystem::resolvePath(resourceID);
	auto it = materials_.find(path);
	if (it != materials_.end())
		return it->second;

	if (fileSystem::getExtension(path) != "syrenmaterial")
	{
		//TODO(log) an error
		return nullptr;
	}

	materials_[path] = std::make_shared<Material>();

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
	//todo save as xml 
}

bool ResourceManager::loadMeshInternal(MeshPtr pMesh,const std::string& file)
{
	if(!fileSystem::fileExist(file))
		return false;

	std::ifstream fileHandler(file);
	if (!fileHandler.is_open()) {
		//std::cerr << "Не удалось открыть файл: " << filename << std::endl;
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

bool ResourceManager::loadMaterialInternal(MaterialPtr pMat, const std::string& file)
{
	if (!fileSystem::fileExist(file))
		return false;

	std::ifstream fileHandler(file);
	if (!fileHandler.is_open()) {
		//std::cerr << "Не удалось открыть файл: " << filename << std::endl;
		return false;
	}

	
	//TODO(load) load vertex(pixel) Shader\Texture
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
