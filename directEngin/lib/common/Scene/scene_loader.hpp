#ifndef __SCENE_LOADER__
#define __SCENE_LOADER__

#include "serialization/xml_parser.hpp"
#include "scene_manager.h"

#include <stdexcept>
#include <iostream>
#include <fstream>



class SceneLoader
{
public:
	SceneLoader(SceneManager& sceneManager) noexcept : _sceneManager(sceneManager) {}

	void save(const std::string& path, const SceneManager& scene);

	bool load(const std::string& path);

	bool loadPool(const XMLNode& node);

	bool unload(const std::string& path)
	{
		throw std::runtime_error("SceneLoader::unload: function Not implemented");
	}

private:

	XMLParser _xmlParser;
	std::vector<std::string> _loadedScenes;
	SceneManager& _sceneManager;
};

#endif //