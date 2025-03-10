#ifndef __PREFAB_ROOT_H__
#define __PREFAB_ROOT_H__

struct NodeData
{
private:
	std::vector<std::string> properties;
};


class NodeTree
{

private:
	size_t tombstone() const noexcept
	{
		return datas.size() + 1;
	}
	std::vector<size_t> nodeToParent;
	std::vector<NodeData> datas;

};

struct Prefab
{
private:
	std::string resourceID;
	std::vector<std::string> meshes;
	std::vector<std::string> textures;
	NodeTree nodes;
};

/// @brief component that attached to hierarcy root of Prefab.
struct PrefabSource
{
	std::string resourceID;
};


#endif