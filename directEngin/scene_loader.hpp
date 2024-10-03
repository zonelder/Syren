#pragma once
#include "../xml_parser.hpp"
#include "common/Scene/scene_manager.h"

class SceneLoader
{
public:
	SceneLoader(SceneManager& sceneManager) noexcept : _sceneManager(sceneManager) {}


	bool load(const std::string& path);

	bool loadPool(const XMLNode& node);

	bool unload(const std::string& path);

private:

	XMLParser _xmlParser;
	std::vector<std::string> _loadedScenes;
	SceneManager& _sceneManager;
};