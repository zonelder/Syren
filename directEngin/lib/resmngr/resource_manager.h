#pragma once
#include <vector>
#include <unordered_map>
#include <memory.h>
#include "components/material.h"
#include "resmngr/fbx_prefab.h"


namespace fileSystem
{
	/// @brief get extension of file
	/// @param file name. or full file name
	/// @return 
	std::string getExtension(const std::string& file);

	/// @brief get local file name without extension
	/// @param file some file name.
	/// @return 
	std::string getFilename(const std::string& file);

	bool fileExist(const std::string& file);

	/// @brief get fullpath from relative path
	/// @param path #relative or fullpath to object;
	/// @return 
	std::string resolvePath(const std::string& path);

	std::string relativePath(const std::string& path);

	std::wstring relativePath(const std::wstring& path);

	std::wstring resolvePath(const std::wstring& path);


	std::string simplifyPath(const std::string& path);
	std::wstring simplifyPath(const std::wstring& path);
}

struct Mesh;
class VertexShader;
class PixelShader;

using MeshPtr			 = std::shared_ptr<Mesh>;
using VertexShaderPtr	 = std::shared_ptr<VertexShader>;
using PixelShaderPtr	 = std::shared_ptr<PixelShader>;
using FbxPrefabPtr		 = std::shared_ptr<FbxPrefab>;
/// @brief class that control loading of Resources. it handles construct\destruct login by itsself.
/// deleting anything that has been returned from ResourceManager emits UB 
class ResourceManager
{
public:
	ResourceManager(Graphics& gfx);

	//@seems to need getResource<T>()
	template<class T>
	using ResourceContainer_t = std::unordered_map<std::string,std::shared_ptr<T>>;

	template<class T>
	using UnhandleResources_t = std::vector<T>;

	//getters
	MeshPtr getMesh(const std::string& resourceID);
	VertexShaderPtr getVertexShader(const std::string& resourceID);
	PixelShaderPtr getPixelShader(const std::string& resorceID);
	MaterialPtr getMaterial(const std::string& resourceID);

	FbxPrefabPtr getFbxPrefab(const std::string& resourceID);

	//build resource on run
	bool saveMesh(const MeshPtr pMesh,const std::string& resourceID);
	bool saveMaterial(const MaterialPtr pMaterial, const std::string& resourceID);

private:
	bool loadSyrenMeshInternal(MeshPtr mesh,const std::string& file);
	bool loadFbxMeshInternal(MeshPtr mesh, const std::string& file);
	bool loadMaterialInternal(MaterialPtr pMat, const std::string& file);


	bool saveMeshInternal(const MeshPtr mesh, const std::string& file);

private:
	Graphics& _gfx;//to init buffers;
	//TODO fonts and textures also should count as resources.
	ResourceContainer_t<Mesh>			meshes_;
	ResourceContainer_t<VertexShader>	vertexShaders_;
	ResourceContainer_t<PixelShader>	pixelShaders_;
	ResourceContainer_t<Material>		materials_;
	ResourceContainer_t<FbxPrefab>		fbxPrefabs_;

	UnhandleResources_t<Mesh>			unhandledMeshes_;
	UnhandleResources_t<Material>		unhandledMaterial_;
};