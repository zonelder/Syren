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
                if os.path.splitext(file)[1] in self.extensions :
                    #print("Serialization: serching for serializable type in file ",file)
                    self.analyze_file(os.path.join(root, file))

        return self.serializable_types,self.errors

    def analyze_file(self, file_path):
        with open(file_path, "r") as file:
            lines = file.readlines()
        print("Serialization::analyze file ",file_path)
        skip_count = 0
        for i, line in enumerate(lines):
            if skip_count > 0 :
                skip_count = skip_count - 1
                continue

            if re.search(r"^SERIALIZE$", line):
                class_name = self.find_type_name(lines, i)
                print("find SERIALIZE typename = ",class_name)# не читает файл
                if not class_name:
                    self.errors.append(f"Ошибка в файле {file_path}({i}): SERIALIZE найден, но класс не определен.")
                    continue
                end_index = self.get_obj_lines(lines,i)
                print("end index count:",end_index)
                if end_index == -1 :
                    self.errors.append(f"Ошибка в файле {file_path}({i}): SERIALIZE найден, но класс не определен.")
                    continue

                skip_count = end_index - i
                self.serializable_types[class_name] = SerializableType(
                    name=class_name,
                    fields={}  # Инициализируем словарь полей
                )

                # После определения типа, ищем поля SERIALIZE_FIELD
                self.extract_fields(lines, class_name,i,end_index)


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

    def get_obj_lines(self,lines,start_index) :
        # Ищем все строки который принадлежат обьекту
        open_braces = 0
        for i in range(start_index, len(lines)):
            line = lines[i].strip()
            old_braces = open_braces
            open_braces += line.count("{")
            open_braces -= line.count("}")
            if old_braces != 0 and open_braces == 0:
                return i+1

        return -1

    def extract_fields(self, lines, class_name,start_index,end_index):
        # Ищем строки с SERIALIZE_FIELD
        for i in range(start_index, end_index):
            line = lines[i].strip()
            print("serch field:",line)# не читает файл
            if not re.search(r"\bSERIALIZE_FIELD\b", line):
                continue
            print("find serializable field in type ",class_name,line)
            # Ищем тип и имя поля
            match = None
            match = re.search(r"SERIALIZE_FIELD\s+(.+?)\s+(\w+)", line)
            if match:
                field_type = match.group(1)
                field_name = match.group(2)
                self.serializable_types[class_name].fields[field_name] = field_type
            else:
                # Ищем тип и имя поля на следующей строке
                if i + 1 < len(lines):
                    next_line = lines[i + 1].strip()
                    match = re.search(r"(.+?)\s+(\w+)", next_line)
                    if match:
                        field_type = match.group(1)
                        field_name = match.group(2)
                        self.serializable_types[class_name].fields[field_name] = field_type
            if not match:
                err =    f"Ошибка в типе {class_name} ({i}): SERIALIZE_FIELD найден, но поле не определено."
                self.errors.append(err)
                continue

    def find_type_name_in_line(self,text) :
        pattern = rf"\b(?:{keywords_pattern})\b\s*(\w+)"
        return re.search(pattern, text)