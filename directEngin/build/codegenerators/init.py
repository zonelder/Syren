import os

from logger import log
from serialize.analyzer import SerializAnalizer 
from serialize.generator import SerializerGenerator


def get_lib_path(cur_dir,levels_up, lib_name="lib"):

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

        for n,type in serializable_types.items() :
            log.info(n)
            for name, t in type.fields.items() :
                str  = f"     {t.type} {name}"
                if(t.is_array()) :
                    str+= f"{t.array_size }"
                log.info(str)

        generator =  SerializerGenerator(libGeneratedFolder)
        generator.generate_from_list(serializable_types.values())


log.info("----------end searching serialize properties-----------")