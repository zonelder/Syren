

#include "scene_loader.hpp"
//WARN this include must be generated in pre-build event. if you have error  with that. check build/init.py and pre-build event isssues
//#include "generation_iternal/agregator.h"
#include "generation_iternal/agregator.h"
#include <format>


void SceneLoader::save(const std::string& path, const SceneManager& scene) {
	// Создаем документ XML
	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("scene");  // Создаем корневой узел "Scene"

	// Сериализуем SceneManager
	Serializer<SceneManager>::serialize(root, scene);

	// Сохраняем документ в файл
	std::ofstream file(path);
	doc.save(file, "\t", pugi::format_default);
}

bool SceneLoader::load(const std::string& path)
{
	if (!_xmlParser.loadFile(path))
		return false;

	auto root = _xmlParser.root();
	auto scene = root.child("scene");

	// Сериализуем SceneManager
	Serializer<SceneManager>::deserialize(scene, _sceneManager);

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
}
