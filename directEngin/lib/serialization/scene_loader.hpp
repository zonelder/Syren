#ifndef __SCENE_LOADER__
#define __SCENE_LOADER__

#include "serialization/xml_parser.hpp"
#include "scene/scene_manager.h"

#include <stdexcept>

class SceneLoader
{
public:
	SceneLoader(SceneManager& sceneManager) noexcept : _sceneManager(sceneManager) {}

	void save(const std::string& path, const SceneManager& scene);

	bool load(const std::string& path);

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