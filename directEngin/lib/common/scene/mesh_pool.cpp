#include "mesh_pool.h"
#include "components/mesh.h"
#include <filesystem>
#include <fstream>
#include "resmngr/xml_node.h"

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
}

Mesh* MeshPool::addMesh(Graphics& gfx,const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices, const MeshIternal::ConstantBuffer2& colors)
{
	meshes_.emplace_back(new  MeshIternal(gfx, vertices, indices, colors));
	wrappers_.emplace_back(meshes_.size()-1,std::size(indices));
	return &(wrappers_.back());
}

MeshIternal* MeshPool::getMesh(Mesh* mesh) const noexcept
{
	return meshes_[mesh->id].get();
}

/// @brief //////////////////////RESOURCE MANAGER ////////////////////



/// @brief getPtr to exact resource.if resource isnot loaded then load it and return to user;
/// @param resourceID full path to resource;
/// @return 
Mesh* ResourceManager::getMesh(const std::string& resourceID)
{
	auto it = meshes_.find(resourceID);
	if (it != meshes_.end())
	{
		return &(it->second);
	}

	if (fileSystem::getExtension(resourceID) != "syrenmesh")
	{
		//TODO(log) an error
		return nullptr;
	}
	meshes_[resourceID] = Mesh();
	auto res = loadMeshIternal(meshes_[resourceID],resourceID);
	if (!res)
	{
		meshes_.erase(resourceID);
		return nullptr;
	}
	return &(meshes_[resourceID]);
}

bool ResourceManager::saveMesh(const Mesh* pMesh, const std::string& resourceID)
{
	if (fileSystem::getExtension(resourceID) != "syrenmesh")
	{
		//TODO(log) an error
		return false;
	}
	//TODO check if this mesh is unhandled mesh and make it handeled;
	return saveMeshIternal(*pMesh, resourceID);
}

bool ResourceManager::loadMeshIternal(Mesh& mesh,const std::string& file)
{
	if(!fileSystem::fileExist(file))
		return false;

	std::ifstream fileHandler(file);
	if (!fileHandler.is_open()) {
		//std::cerr << "Не удалось открыть файл: " << filename << std::endl;
		return false;
	}
	std::string line;
	size_t vertexCount,indexCount,colorCount;

	if (!fileHandler >> vertexCount)
	{
		return false;
	}
	mesh.vertexes.reserve(vertexCount);
	for (size_t i = 0; i < vertexCount; ++i)
	{
		auto& v = mesh.vertexes.emplace_back();
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

	mesh.indices.reserve(indexCount);

	for (size_t i = 0; i < indexCount; ++i)
	{
		auto& index = mesh.indices.emplace_back();
		if (!(fileHandler >> index))
		{
			return false;
		}
	}

	if (!(fileHandler >> colorCount))
	{
		return false;
	}

	mesh.colors.reserve(colorCount);

	for (size_t i = 0; i < colorCount; ++i)
	{
		auto& color = mesh.colors.emplace_back();
		if (!(fileHandler >> color.r >> color.g >> color.b >> color.a))
		{
			return false;
		}
	}

	fileHandler.close();
	mesh.resourceID = file;

	return true;

}

bool ResourceManager::saveMeshIternal(const Mesh& mesh, const std::string& filename)
{
	std::ofstream file(filename);
	if (!file.is_open()) 
	{
		return false;
	}

	file << mesh.vertexes.size() << std::endl;
	for (const auto& vertex : mesh.vertexes) 
	{
		auto pos = vertex.position.m128_f32;
		file << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
		file << vertex.uv.x << " " << vertex.uv.y << std::endl;
	}

	file << mesh.indices.size() << std::endl;
	for (const auto& index : mesh.indices) 
	{
		file << index << std::endl;
	}

	file << mesh.colors.size() << std::endl;
	for (const auto& color : mesh.colors) 
	{
		file << color.r << " " << color.g << " " << color.b << " " << color.a << std::endl;
	}

	file.close();
	return true;
}
