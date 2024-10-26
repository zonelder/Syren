#include "scene_loader.hpp"
#include <format>
#include "generation_iternal/scene_serializer.h"

/*
void SceneLoader::save(const std::string& path)
{
}
bool SceneLoader::load(const std::string& path)
{
	if (!_xmlParser.loadFile(path))
		return false;

	auto root = _xmlParser.root();
	auto pools = root.child("pools");

	return true;
}

bool SceneLoader::loadPool(const XMLNode& node)
{
	auto guidNode = node.child("guid");
	if (!guidNode)
	{
		std::cerr<<std::format("Error: error occured while parsing the scene. Pool {} has no guid section.\n",node.value<std::string>());
		return false;
	}

	auto guid = guidNode.value<std::string>();
	if (guid.empty())
	{
		std::cerr<< std::format("Error: error occured while parsing the scene. Pool {} has empty guid.\n",node.value<std::string>());
	}
	return false;
}*/
