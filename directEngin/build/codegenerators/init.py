from serialize.analyzer import SerializAnalizer 
import os

def get_lib_path(cur_dir,levels_up, lib_name="lib"):
    """
    Возвращает путь к папке lib, находящейся на указанном количестве уровней выше текущей папки.

    Args:
        levels_up (int): Количество уровней выше текущей папки.
        lib_name (str, optional): Имя папки lib. По умолчанию "lib".

    Returns:
        str: Путь к папке lib.
    """

    for _ in range(levels_up):
        cur_dir = os.path.dirname(cur_dir)

    lib_path = os.path.join(cur_dir, lib_name)
    return lib_path

print("----------start serching serialize properties----------")
analyzer = SerializAnalizer(get_lib_path(__file__,3), [".cpp", ".h"])
serializable_types, errors = analyzer.find_serializable_types()


if errors and len(errors) >0 :
    print("find errors:")
    for error in errors:
        print(error)
else:
    if(len(serializable_types.items()) == 0) :
        print("there is not any user defined serializable types.")
    else :
        print("Serializable types:")
        for type_name, type_info in serializable_types.items():
            print(f"  {type_info.name}")
            print(f"    fields:")
            for field_name, field_type in type_info.fields.items():
                print(f"      {field_name}: {field_type}")


print("----------end searching serialize properties-----------")