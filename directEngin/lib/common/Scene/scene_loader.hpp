#pragma once
#include "serialization/xml_parser.hpp"
#include "common/Scene/scene_manager.h"

//WARN this include must be generated in pre-build event. if you have error  with that. check build/init.py and pre-build event isssues
#include "generation_iternal/agregator.h"


class SceneLoader
{
public:
	SceneLoader(SceneManager& sceneManager) noexcept : _sceneManager(sceneManager) {}

	void save(const std::string& path);

	bool load(const std::string& path);

	bool loadPool(const XMLNode& node);

	bool unload(const std::string& path);

private:

	XMLParser _xmlParser;
	std::vector<std::string> _loadedScenes;
	SceneManager& _sceneManager;
};