#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "material.h"
#include "fbx_prefab.h"

#include <vector>
#include <unordered_map>
#include <memory>

struct Mesh;
class VertexShader;
class PixelShader;

using MeshPtr			 = std::shared_ptr<Mesh>;
using VertexShaderPtr	 = std::shared_ptr<VertexShader>;
using PixelShaderPtr	 = std::shared_ptr<PixelShader>;
using TexturePtr		 = std::shared_ptr<Texture>;
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

	MeshPtr getProceduralMesh(const std::string& resourceID);
	VertexShaderPtr getVertexShader(const std::string& resourceID);
	PixelShaderPtr getPixelShader(const std::string& resorceID);
	MaterialPtr getMaterial(const std::string& resourceID);

	TexturePtr getTexture(const std::string& resourceID);

	TexturePtr getDefaultTexture();

	FbxPrefabPtr getFbxPrefab(const std::string& resourceID);

	//build resource on run
	bool saveMesh(const MeshPtr pMesh,const std::string& resourceID);
	bool saveMaterial(const MaterialPtr pMaterial, const std::string& resourceID);

private:

	bool loadSyrenMeshInternal(MeshPtr mesh,const std::string& file);
	bool loadMaterialInternal(MaterialPtr pMat, const std::string& file);
	bool saveMeshInternal(const MeshPtr mesh, const std::string& file);

private:
	Graphics& _gfx;//to init buffers;
	ResourceContainer_t<Mesh>			meshes_;
	ResourceContainer_t<Mesh>			proceduralMesh_;
	ResourceContainer_t<VertexShader>	vertexShaders_;
	ResourceContainer_t<PixelShader>	pixelShaders_;
	ResourceContainer_t<Material>		materials_;
	ResourceContainer_t<Texture>		textures_;
	ResourceContainer_t<FbxPrefab>		fbxPrefabs_;

	UnhandleResources_t<Mesh>			unhandledMeshes_;
	UnhandleResources_t<Material>		unhandledMaterial_;
};

#endif