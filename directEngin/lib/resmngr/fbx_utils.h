#ifndef __FBX_UTILS__
#define __FBX_UTILS__

#include <fbxsdk.h>
#include "components/mesh.h"
#include "math/quaternion.h"

using MeshPtr = std::shared_ptr<Mesh>;
//class Color;

namespace fbx_utils
{

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

	Color toColor(FbxColor color);

	DirectX::XMFLOAT2 toVector2(const FbxVector2& v);

	Vector3 toVector3(const FbxVector4& v);
	DirectX::XMFLOAT4 toVector4(const FbxVector4& v);
	Quaternion toVector4(const FbxQuaternion& v);




	bool isMesh(FbxNode* node);

	bool loadMeshFromNode(FbxMesh* fbxMesh, MeshPtr mesh, const std::string& file, const FbxRAII<FbxManager>& manager);
}




#endif