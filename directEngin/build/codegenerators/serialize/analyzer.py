import os
import re

class SerializableType:
    def __init__(self,name,fields) :
        self.name = name
        self.fields = fields


class SerializAnalizer:
    def __init__(self, root_dir, extensions):
        print("create serialize analyzer in folders (" + root_dir +"). with extensions" + str(extensions))
        self.root_dir = root_dir
        self.extensions = extensions
        self.serializable_types = {}
        self.errors = {}


    def find_serializable_types(self):
        print("serching serializable types in ",self.root_dir)
        for root,dirnames, files in os.walk(self.root_dir):
            for file in files:
                if os.path.splitext(file)[1] in self.extensions:
                    #print("Serialization: serching for serializable type in file ",file)
                    self.analyze_file(os.path.join(root, file))

        return self.serializable_types,self.errors

    def analyze_file(self, file_path):
        with open(file_path, "r") as file:
            lines = file.readlines()
        print("Serialization::analyze file ",file_path)
        for i, line in enumerate(lines):
            print("find SERIALIZABLE definition",line)# не читает файл
            if re.search(r"SERIALIZABLE", line):
                class_name = self.find_class_name(lines, i)
                if class_name:
                    self.serializable_types[class_name] = SerializableType(
                        name=class_name,
                        fields={}  # Инициализируем словарь полей
                    )
                    # После определения типа, ищем поля SERIALIZE_FIELD
                    self.extract_fields(lines, class_name)
                else:
                    # Ошибка: SERIALIZABLE найден, но класс не найден
                    self.errors.append(f"Ошибка в файле {file_path}({i}): SERIALIZABLE найден, но класс не определен.")
                    break

    def find_class_name(self, lines, line_index):
        # Ищем класс на текущей и следующей строке
        match = re.search(r"class\s+(\w+)", lines[line_index].strip())
        if match:
            return match.group(1)
        elif line_index + 1 < len(lines):
            match = re.search(r"class\s+(\w+)", lines[line_index + 1].strip())
            if match:
                return match.group(1)

        return None  # Класс не найден

    def extract_fields(self, lines, class_name,line_index):
        # Ищем строки с SERIALIZE_FIELD
        for i in range(line_index, len(lines)):
            line = lines[i].strip()
            if not re.search(r"SERIALIZE_FIELD", line):
                continue
                            # Ищем тип и имя поля
            match = re.search(r"(\w+)\s+(\w+)", line)
            if match:
                field_type = match.group(1)
                field_name = match.group(2)
                self.serializable_types[class_name].fields[field_name] = field_type
            else:
                # Ищем тип и имя поля на следующей строке
                if i + 1 < len(lines):
                    next_line = lines[i + 1].strip()
                    match = re.search(r"(\w+)\s+(\w+)", next_line)
                    if match:
                        field_type = match.group(1)
                        field_name = match.group(2)
                        self.serializable_types[class_name].fields[field_name] = field_type
            if not match:
                self.errors.append(
                    f"Ошибка в файле {file_path}({i}): SERIALIZE_FIELD найден, но имя поля не определено."
                )
                continue