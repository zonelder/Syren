import os
import logging
import sys

logLevel = logging.INFO

# Создаем глобальный логгер
log = logging.getLogger("MyScriptLogger")
log.setLevel(logLevel)  # Уровень по умолчанию INFO
# Формат вывода сообщений
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

# Вывод в консоль
console_handler = logging.StreamHandler(sys.stdout)
console_handler.setLevel(logLevel)  # Уровень по умолчанию INFO
console_handler.setFormatter(formatter)
log.addHandler(console_handler)

#end global definitions


from serialize.analyzer import SerializAnalizer 
from serialize.generator import SerializerGenerator



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

log.info("----------start serching serialize properties----------")
libPath = get_lib_path(os.path.abspath(__file__),3)

libGeneratedFolder = os.path.join(libPath,"generation_iternal")
mainSerializerFile = "serializer_gen.h" # этот файлит будет собирать в себе все сериализаторы 
analyzer = SerializAnalizer(libPath, [".cpp", ".h"])
serializable_types, errors = analyzer.find_serializable_types()


if errors and len(errors) >0 :
    log.error("find errors:" + '\n'.join(errors))
else:
    if(len(serializable_types.items()) == 0) :
        log.info("there is not any user defined serializable types.")
    else :

        generator =  SerializerGenerator(libGeneratedFolder)
        generator.generate_from_list(serializable_types.values())


log.info("----------end searching serialize properties-----------")