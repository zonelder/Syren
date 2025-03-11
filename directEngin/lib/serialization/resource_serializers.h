#ifndef __RESOURCE_SERIALIZERS_H__
#define __RESOURCE_SERIALIZERS_H__

#include "default.h"
#include "resmngr/resource_manager.h"
#include "resmngr/xml_node.h"
#include "graphics/Drawable/BindComponent/vertex_shader.h"
#include "graphics/Drawable/BindComponent/pixel_shader.h"
#include "graphics/Drawable/BindComponent/texture.h"
#include <string>

template<typename T>
struct Serializer
{
    static void serialize(XMLNode node, const T& value)
    {
        node.setValue(value);
    }

    static T deserialize(XMLNode node)
    {
        return node.value(T());
    }

    static void deserialize(XMLNode node, T& value)
    {
        value = node.value(T());
    }
};

template<>
struct Serializer<MeshPtr>
{
    static void serialize(XMLNode node, const MeshPtr& value)
    {
        if (value && !(value->resourceID.empty()))
        {
            Serializer<std::string>::serialize(node.saveGetChild("resourceID"), value->resourceID);
        }
    }

    static MeshPtr deserialize(XMLNode node)
    {
        std::string resourceID = Serializer<std::string>::deserialize(node.child("resourceID"));
        if (!resourceID.empty())
            return SceneContext::resources().getMesh(resourceID);
        return nullptr;
    }

    static void deserialize(XMLNode node, MeshPtr& value)
    {
        value = deserialize(node);
    }
};

template<>
struct Serializer<MaterialPtr>
{
    static void serialize(XMLNode node, const MaterialPtr& value)
    {
        if (value && !(value->resourceID.empty()))
        {
            
            Serializer<std::string>::serialize(node.saveGetChild("resourceID"), value->resourceID);
        }
    }

    static MaterialPtr deserialize(XMLNode node)
    {
        std::string resourceID = Serializer<std::string>::deserialize(node.child("resourceID"));
        if (!resourceID.empty())
            return SceneContext::resources().getMaterial(resourceID);
        return nullptr;
    }

    static void deserialize(XMLNode node, MaterialPtr& value)
    {
        value = deserialize(node);
    }
};

// Specialization for VertexShaderPtr
template<>
struct Serializer<VertexShaderPtr>
{
    static void serialize(XMLNode node, const VertexShaderPtr& value)
    {
        if (value)
            Serializer<std::string>::serialize(node.saveGetChild("resourceID"), value->resourceID());
        else
            Serializer<std::string>::serialize(node.saveGetChild("resourceID"), "");
    }

    static VertexShaderPtr deserialize(XMLNode node)
    {
        std::string resourceID = Serializer<std::string>::deserialize(node.child("resourceID"));
        if (!resourceID.empty())
            return SceneContext::resources().getVertexShader(resourceID);
        return nullptr;
    }

    static void deserialize(XMLNode node, VertexShaderPtr& value)
    {
        value = deserialize(node);
    }
};

// Specialization for PixelShaderPtr
template<>
struct Serializer<PixelShaderPtr>
{
    static void serialize(XMLNode node, const PixelShaderPtr& value)
    {
        if (value)
            Serializer<std::string>::serialize(node.saveGetChild("resourceID"), value->resourceID());
        else
            Serializer<std::string>::serialize(node.saveGetChild("resourceID"), "");
    }

    static PixelShaderPtr deserialize(XMLNode node)
    {
        std::string resourceID = Serializer<std::string>::deserialize(node.child("resourceID"));
        if (!resourceID.empty())
            return SceneContext::resources().getPixelShader(resourceID);
        return nullptr;
    }

    static void deserialize(XMLNode node, PixelShaderPtr& value)
    {
        value = deserialize(node);
    }
};

/*
// Specialization for TexturePtr
template<>
struct Serializer<TexturePtr>
{
    static void serialize(XMLNode node, const TexturePtr& value)
    {
        if (value && !value->getPath().empty())
            Serializer<std::wstring>::serialize(node.saveGetChild("resourceID"), value->getPath());
        else
            Serializer<std::wstring>::serialize(node.saveGetChild("resourceID"), L"");
    }

    static TexturePtr deserialize(XMLNode node)
    {
        std::string resourceID = Serializer<std::string>::deserialize(node.child("resourceID"));
        if (!resourceID.empty())
            return SceneContext::resManager().getTexture(resourceID);
        return nullptr;
    }

    static void deserialize(XMLNode node, TexturePtr& value)
    {
        value = deserialize(node);
    }
};
*/
#endif // __RESOURCE_SERIALIZERS_H__