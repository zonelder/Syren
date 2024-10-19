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

//#include "../resmngr/xml_node.h"
//#include "..\{type.path}"

template<class T>
struct Serializer;

template <>
struct Serializer<{type.name}> {{
    static void serialize(XMLNode node, const {type.name}& value) {{
""")
            for field_name, field_type in type.fields.items():
                f.write(f"        Serializer<{field_type}>::serialize(node.saveGetChild(\"{field_name}\"), value.{field_name});\n")
            f.write(f"""    }}

    static {type.name} deserialize(const XMLNode node) {{
        {type.name} value;
""")

            for field_name, field_type in type.fields.items():
                f.write(f"        if ( auto fieldNode = node.child(\"{field_name}\"))\n")
                f.write(f"            value.{field_name} = Serializer<{field_type}>::deserialize(fieldNode);\n")

            f.write(f"""        return value;
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
            # Инклуды файлов с определением типов
            for type in types:
                f.write(f'#include "..\{type.path}"\n')

            # Инклуд файла с реализацией базовых сериализаторов
            f.write('\n#include "serialization/default.h"\n\n')

            # Инклуды файлов сериализаторов типов
            for type in types:
                f.write(f'#include "{type.name}_serializer.h"\n')
