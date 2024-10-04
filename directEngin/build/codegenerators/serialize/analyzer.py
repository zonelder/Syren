import os
import re


type_key_words = [
    "class",
    "struct",
]

keywords_pattern = "|".join(type_key_words)

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
        self.errors = []


    def find_serializable_types(self):
        print("serching serializable types in ",self.root_dir)
        for root,dirnames, files in os.walk(self.root_dir):
            for file in files:
                if os.path.splitext(file)[1] in self.extensions and os.path.splitext(file)[1] == ".h":
                    #print("Serialization: serching for serializable type in file ",file)
                    self.analyze_file(os.path.join(root, file))

        return self.serializable_types,self.errors

    def analyze_file(self, file_path):
        file_name_with_ext = os.path.basename(file_path)
        if file_name_with_ext != "text.h" :
            return
        with open(file_path, "r") as file:
            lines = file.readlines()
        print("Serialization::analyze file ",file_path)
        print("lines:",lines)
        for i, line in enumerate(lines):

            if "SERIALIZE" in line:
                print("find SERIALIZE definition",line)# не читает файл
                class_name = self.find_type_name(lines, i)
                print("find SERIALIZE typename = ",class_name)# не читает файл
                if not class_name:
                    # Ошибка: SERIALIZABLE найден, но класс не найден
                    self.errors.append(f"Ошибка в файле {file_path}({i}): SERIALIZE найден, но класс не определен.")
                    continue
                self.serializable_types[class_name] = SerializableType(
                    name=class_name,
                    fields={}  # Инициализируем словарь полей
                )
                # После определения типа, ищем поля SERIALIZE_FIELD
                self.extract_fields(lines, class_name,i)


    def find_type_name(self, lines, line_index):
        # Ищем класс на текущей и следующей строке
        match = self.find_type_name_in_line(lines[line_index].strip())
        if match:
            return match.group(1)
        elif line_index + 1 < len(lines):
            match = self.find_type_name_in_line(lines[line_index + 1].strip())
            if match:
                return match.group(1)

        return None  # Класс не найден

    def find_type_name_in_line(self,text) :
        pattern = rf"\b(?:{keywords_pattern})\b\s*(\w+)"
        return re.search(pattern, text)

    def extract_fields(self, lines, class_name,line_index):
        # Ищем строки с SERIALIZE_FIELD
        i = line_index
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
        return i