![C++](https://img.shields.io/badge/language-C++-blue.svg)
![x64](https://img.shields.io/badge/arch-x64-green.svg)
![platforms](https://img.shields.io/badge/platforms-Windows-blue.svg)

> **Warning:** Keep in mind - work in progress. API can see significant changes in future.

## About

Syren is a game engine based on DirectX 11 that provides a framework for accelerating 2D/3D game development. Built on Entity Component System (ECS) architecture, Syren offers lightweight tools for manipulating objects with high-quality rendering.

## Features

- **Entity Component System (ECS)** architecture
- **DirectX 11** rendering pipeline
- **View Iteration** for efficient entity processing
- **System-based** game logic organization
- **Resource Management** with FBX model support
- **Scene Management** with serialization

## Getting Started

Here's a minimal example demonstrating the component system and view iteration:

```cpp
#include "App.h"
#include "systems/render_system.h"
#include "components/transform.h"
#include "components/render.h"

// Custom system example
class RotationSystem : public BaseSystem {
public:
    void onUpdate(SceneManager& scene, float time) override {
        // View iteration - process only entities with Transform  components
        for(auto& [entt,transform] : scene.view<Transform>())
        {
              transform.rotation.y += 1.0f * dt;
        }
    }
};

void App::onInit()
{
    // Register systems
    _systemManager.add<RotationSystem>();  // Add our custom rotation system
    
    // Create an entity
    auto& entity = _scene.createEntity();

    // Add components
    auto& transform = _scene.addComponent<Transform>(entity);
    transform.position = {0.0f, 0.0f, 0.0f};
}
```

Also you can Save\Load scene via-serialize reflection:
```cpp
//create a serializable type
SERIALIZABLE
struct MyComponent
{
  SERIALIZE_FIELD float serializable_field;
  int non_serializable_field;
}
auto entt =  scene.createEntity();
scene.addComponent<MyComponent>(entt);
SceneLoader sl(scene);
//save all serializable components that was added to scene
sl.save("my_scene_file.syrenscene");
//clear up scene
scene.clear();

//recover scene from file
sl.load("me_scene_file.syrenscene")
```


## Project Structure

- **directEngin/lib/** - Core library files
  - **common/** - Base classes including App and custom_app
  - **components/** - Component definitions for ECS
  - **systems/** - Game systems implementation
  - **scene/** - Scene management
  - **graphic/** - DirectX 11 rendering implementation
 
## References
- DirectX 11 - Microsoft's graphics API for Windows
- pugixml - Light-weight XML processing library
- HLSL - High-Level Shading Language for DirectX
- Windows SDK - Development kit for Windows applications
- FBXSDK -Development kit for managinf fbx file format

## Documentation

Documentation is available in the `docs/` directory, generated using Doxygen.

## License

This project is licensed under the MIT License.

---

*Syren - DirectX 11 game engine with powerful ECS architecture*
