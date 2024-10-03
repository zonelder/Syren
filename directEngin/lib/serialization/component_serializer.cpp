#include "component_serializer.h"
#include "common/Scene/scene_manager.h"




template<class T>
serializeHasher<T>::serializeHasher()
{
    serializer<SceneManager>::s_poolDeserializer[s_hash] = [](XMLNode poolNode, SceneManager& scene) {poolSerializer<T>::deserialize(poolNode, scene); };
};