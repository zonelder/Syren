#pragma once
#include <vector>
#include <unordered_map>
#include <memory.h>
#include "components/mesh_iternal.h"


using meshPtrVector = std::vector<std::shared_ptr<MeshInternal>>;

struct Mesh;
class VertexShader;
class PixelShader;
struct Material;

using MeshPtr = std::shared_ptr<Mesh>;
using VertexShaderPtr = std::shared_ptr<VertexShader>;
using PixelShaderPtr = std::shared_ptr<PixelShader>;
/*
class MeshPool
{
public:


	/// @brief create new mesh and return ptr to it
	/// @return 
	Mesh* addMesh(
		Graphics& gfx,
		const std::vector<Vertex>& vertices,
		const std::vector<unsigned short>& indices,
		const MeshInternal::ConstantBuffer2& colors
	);

	MeshInternal* getMesh(Mesh* mesh) const noexcept;

private:

	meshPtrVector meshes_;
	std::vector<Mesh> wrappers_;
};
*/
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

	//build resource on run
	bool saveMesh(const MeshPtr pMesh,const std::string& resourceID);
	bool saveMaterial(const MaterialPtr pMaterial, const std::string& resourceID);

private:
	bool loadMeshInternal(MeshPtr mesh,const std::string& file);
	bool loadMaterialInternal(MaterialPtr pMat, const std::string& file);


	bool saveMeshInternal(const MeshPtr mesh, const std::string& file);

private:
	Graphics& _gfx;//to init buffers;
	//TODO fonts and textures also should count as resources.
	ResourceContainer_t<Mesh>			meshes_;
	ResourceContainer_t<VertexShader>	vertexShaders_;
	ResourceContainer_t<PixelShader>	pixelShaders_;
	ResourceContainer_t<Material>		materials_;
	

	UnhandleResources_t<Mesh>			unhandledMeshes_;
	UnhandleResources_t<Material>		unhandledMaterial_;
};