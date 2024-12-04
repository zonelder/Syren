#pragma once
#include <vector>
#include <unordered_map>
#include <memory.h>
#include "components/mesh_iternal.h"


using meshPtrVector = std::vector<std::shared_ptr<MeshIternal>>;

struct Mesh;
class VertexShader;
class PixelShader;
class Material;

class MeshPool
{
public:


	/// @brief create new mesh and return ptr to it
	/// @return 
	Mesh* addMesh(
		Graphics& gfx,
		const std::vector<Vertex>& vertices,
		const std::vector<unsigned short>& indices,
		const MeshIternal::ConstantBuffer2& colors
	);

	MeshIternal* getMesh(Mesh* mesh) const noexcept;

private:

	meshPtrVector meshes_;
	std::vector<Mesh> wrappers_;
};

/// @brief class that control loading of Resources. it handles construct\destruct login by itsself.
/// deleting anything that has been returned from ResourceManager emits UB 
class ResourceManager
{
public:
	//@seems to need getResource<T>()
	template<class T>
	using ResourceContainer_t = std::unordered_map<std::string,T>;

	template<class T>
	using UnhandleResources_t = std::vector<T>;

	//getters
	Mesh* getMesh(const std::string& resourceID);
	VertexShader* getVertexShader(const std::string& resourceID);
	PixelShader* getPixelShader(const std::string& resorceID);
	Material* getMaterial(const std::string& resourceID);


	//build resource on run
	bool saveMesh(const Mesh* pMesh,const std::string& resourceID);
	bool saveMaterial(const Material* pMaterial, const std::string& resourceID);

private:
	bool loadMeshIternal(Mesh& mesh,const std::string& file);
	bool saveMeshIternal(const Mesh& mesh, const std::string& file);

private:
	Graphics& gfx_;//to init buffers;

	ResourceContainer_t<Mesh>			meshes_;
	ResourceContainer_t<VertexShader>	vertexShaders_;
	ResourceContainer_t<PixelShader>	pixelShaders_;
	ResourceContainer_t<Material>		materials_;
	

	UnhandleResources_t<Mesh>			unhandledMeshes_;
	UnhandleResources_t<Material>		unhandledMaterial_;
};