
#include "scene_loader.hpp"
//WARN this include must be generated in pre-build event. if you have error  with that. check build/init.py and pre-build event isssues
#include "generation_iternal/agregator.h"

#include <format>
#include <iostream>
#include <fstream>


void SceneLoader::save(const std::string& path, const SceneManager& scene)
{
	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("scene");  // Создаем корневой узел "Scene"

	Serializer<SceneManager>::serialize(root, scene);

	std::ofstream file(path);
	doc.save(file, "\t", pugi::format_default);
}

bool SceneLoader::load(const std::string& path)
{
	if (!_xmlParser.loadFile(path))
		return false;

	auto scene = _xmlParser.root();
	Serializer<SceneManager>::deserialize(scene, _sceneManager);

	return true;
}
