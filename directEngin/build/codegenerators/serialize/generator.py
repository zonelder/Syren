import os
from logger import log
from serialize.analyzer import SerializableType

class SerializerGenerator:
    def __init__(self,output_path:str):
        self.output_path = output_path

    def generate_from_list(self, serializableTypesList):
        # Создаем папку, если она не существует
        log.debug("start generation for serializable types")
        if not os.path.exists(self.output_path):
            os.makedirs(self.output_path)
            print("create folder ",self.output_path)

        # Генерируем код для каждого типа
        for type in serializableTypesList:
            self.generate(type)

        self.generate_pools_serializer_header(serializableTypesList,os.path.join(self.output_path,"scene_serializer.h"))
        self.generate_pools_serializer_cpp(serializableTypesList,os.path.join(self.output_path,"scene_serializer.ipp"),"agregator.h")
        self.generate_aggregator_file(serializableTypesList,os.path.join(self.output_path,"agregator.h"))

    def generate(self,type : SerializableType) :
                # Определяем имя файла
        file_name = f"{type.name}_serializer.h"
        log.info(f"generating Serializer for type {type.name} in file {file_name}")
                # Создаем путь к файлу
        file_path = os.path.join(self.output_path, file_name)

        # Создаем файл (с перезаписью, если файл уже существует)
        with open(file_path, "w") as f:
            # Заголовок файла
            f.write(f"""#ifndef {type.name.upper()}_SERIALIZER_HPP
#define {type.name.upper()}_SERIALIZER_HPP

#include <iostream>
#include <fstream>
#include <string>

template<class T>
struct Serializer;

template <>
struct Serializer<{type.name}> {{
    static void serialize(XMLNode node, const {type.name}& value) 
    {{
""")
            for field_name, field_type in type.fields.items():
                f.write(self.serialize_field_str(field_type))
            f.write(f"""    }}

    static {type.name} deserialize(const XMLNode node) 
    {{
        {type.name} value;
        deserialize(node,value);
        return value;
    }}
    
    static void deserialize(const XMLNode node,{type.name}& value)
    {{
""")
            for field_name, field_type in type.fields.items():
                f.write(self.deserialize_field_str(field_type))

            f.write(f"""
    }}
}};

#endif // {type.name.upper()}_SERIALIZER_HPP
""")

    def generate_aggregator_file(self,types, output_path: str):
        """
        Генерирует файл-агрегатор для всех сериализуемых типов.

        Args:
            types (list[SerializableType]): Список сериализуемых типов.
            output_path (str): Путь к файлу-агрегатору.
        """

        with open(output_path, "w") as f:
            f.write("#ifndef __CG_SERIALIZE_AGREGATOR__\n#define __CG_SERIALIZE_AGREGATOR__\n")
            # Инклуды файлов с определением типов
            for type in types:
                f.write(f'#include "..\{type.path}"\n')

            # Инклуд файла с реализацией базовых сериализаторов
            f.write('\n#include "serialization/default.h"\n\n')

            # Инклуды файлов сериализаторов типов
            for type in types:
                f.write(f'#include "{type.name}_serializer.h"\n')

            f.write('#include "scene_serializer.h"\n')
            f.write('#include "scene_serializer.ipp"\n')
            f.write('#endif')

    def serialize_field_str(self,field_type) :
        if not field_type.is_array() :
            return f"        Serializer<{field_type.type}>::serialize(node.saveGetChild(\"{field_type.name}\"), value.{field_type.name});\n"
        res = f"""        {{
            auto arrayNode = node.saveGetChild(\"array_of_{field_type.name}\");
            auto size = std::distance(std::begin(value.{field_type.name}),std::end(value.{field_type.name}));
            std::vector<XMLNode> childs;
            arrayNode.childs(\"{field_type.name}\",childs,(int)size);

            for(ptrdiff_t i = 0 ;i < size ;++i )
            {{
                Serializer<{field_type.type}>::serialize(childs[i],value.{field_type.name}[i]);
            }}

        }}\n"""

        return res



    def deserialize_field_str(self,field_type) :
        if not field_type.is_array() :
            res = f"        if ( auto fieldNode = node.child(\"{field_type.name}\"))\n"

            res += f"           Serializer<{field_type.type}>::deserialize(fieldNode,value.{field_type.name});\n"
            return res

            #TODO we may add Defaut property implementation here
        res = f"""
        if ( auto fieldNode = node.child(\"array_of_{field_type.name}\"))
        {{
            std::vector<XMLNode> childs;
            fieldNode.childs(\"{field_type.name}\",childs);
            for( std::vector<XMLNode>::size_type i = 0 ; i < childs.size() ; ++i )
            {{
                value.{field_type.name}[i] = Serializer<{field_type.type}>::deserialize(childs[i]);
            }}

        }}\n"""
        return res

    def generate_pools_serializer_header(self,types,output_path) :
        with open(output_path, "w") as f:
            f.write(f"""
#ifndef __SCENE_SERIALIZE_GENERATED__
#define __SCENE_SERIALIZE_GENERATED__

#include <functional>
#include <string>
#include <unordered_map>

#include "common/Scene/scene_manager.h"

struct PoolSerializer
{{
    using Function_t = std::function<void(XMLNode,SceneManager&)>;
    using Function_const_t = std::function<void(XMLNode,const SceneManager&)>;
    static const std::unordered_map<std::string,Function_const_t> serialize_pool;
    static const std::unordered_map<std::string,Function_t> deserialize_pool;
    static const std::unordered_map<ComponentID,std::string> component_id_to_name;

    static void serialize(XMLNode poolsNode,const SceneManager& manager)
    {{
        if(poolsNode.identifier() != "pools")
        {{
            std::cerr << "PoolSerializer::serialize: get invalid node to serialization.\\n";
            return;
        }}

        for(const auto& [compID,pool] : manager.pools())
        {{
            auto it = component_id_to_name.find(compID);
            if(it == component_id_to_name.end())
                continue;//unserializable type
            const auto& typeName = it->second;
            serialize_pool.find(typeName)->second(poolsNode.saveGetChild(typeName),manager);
        }}
    }}

    static void deserialize(XMLNode poolsNode,SceneManager& manager)
    {{
        if(poolsNode.identifier() != "pools")
        {{
            std::cerr << "PoolSerializer::serialize: get invalid node to serialization.\\n";
            return;
        }}

        for (auto& pool : poolsNode.childs())
        {{
            auto it = deserialize_pool.find(pool.identifier());
            if(it == deserialize_pool.end())
            {{
                std::cerr << "PoolSerializer::deserialize: attempt to deserialize pool but exact type is unknown."<<std::endl;
                continue;
            }}
            it->second(pool,manager);
        }}


    }}
}};

template<>
struct Serializer<SceneManager>
{{
    static void serialize(XMLNode node,const SceneManager& manager)
    {{
        auto poolsNode = node.saveGetChild("pools");
        PoolSerializer::serialize(poolsNode,manager);
    }}

    static void deserialize(XMLNode node,SceneManager& manager)
    {{
        auto poolsNode = node.saveGetChild("pools");
        PoolSerializer::deserialize(poolsNode,manager);
    }}
}};

#endif
""")
    def generate_pools_serializer_cpp(self,types,path,agreg) :

        with open(path, "w") as f:
           # f.write(f"""#include "{agreg}"\n""")
            f.write(f"""
#ifndef __SCENE_SERIALIZER_IPP__
#define __SCENE_SERIALIZER_IPP__
#include <limits>
#include <format>
#include <cstdio>

#undef max

size_t getEntityID(const std::string& entityStr)
{{
    constexpr size_t separator_prefix_size = 7;// we expect string like "Entity_10" if its not save - something get wront;
    std::string numberString = entityStr.substr(separator_prefix_size);
    size_t number = std::stoi(numberString);//Possibly can throw
    return number;
}}

std::string getEntityStr(size_t id)
{{
    return std::format("Entity_{{}}",id);
}}

template<typename T>
constexpr auto get_pool_loader()
{{
    return [](XMLNode node,SceneManager& manager)
    {{
        manager.getPool<T>();// in case pool did not load earlier this method create it;
        for(auto enttNode : node.childs())
        {{
            auto enttID = getEntityID(enttNode.identifier());
            if(enttID == std::numeric_limits<size_t>::max())
            {{
                std::cerr<< "cant read entity data in pool"<<std::endl;
                continue;
            }}
            auto dataNode  = enttNode.saveGetChild("data");
            auto& comp = manager.addComponent<T>(enttID);
            Serializer<T>::deserialize(dataNode,comp);
        }}
    }};
}};



template<typename T>
constexpr auto get_pool_saver()
{{
    return [](XMLNode node,const SceneManager& manager)
    {{
       // manager.getPool<T>();// in case pool did not load earlier this method create it;
       auto& pool = manager.getPool<T>();
       auto eIt =  pool.ebegin();
       auto eend = pool.eend();
       auto dataIt = pool.begin();

       for(;eIt != eend;++eIt,++dataIt)
       {{
            auto enttNode =  node.saveGetChild(getEntityStr(*eIt));
            auto dataNode = enttNode.saveGetChild("data");
            Serializer<T>::serialize(dataNode,*dataIt);
        }}
    }};
}};
""")
            f.write("""const std::unordered_map<std::string,PoolSerializer::Function_const_t> PoolSerializer::serialize_pool\n{""")

            for type in types :
                f.write(f"""\n        {{\"{type.name}\",get_pool_saver<{type.name}>()}},""")
            f.write(f"""\n}};\nconst std::unordered_map<std::string,PoolSerializer::Function_t> PoolSerializer::deserialize_pool\n{{""")

            for type in types :
                f.write(f"""\n        {{\"{type.name}\",get_pool_loader<{type.name}>()}},""")
            f.write(f"""\n}};\nconst std::unordered_map<ComponentID,std::string> PoolSerializer::component_id_to_name\n{{""")

            for type in types :
                f.write(f"""\n        {{Family::type_id<{type.name}>(),"{type.name}\"}},""")
            f.write(f"""\n}};
#endif//            
            """);

