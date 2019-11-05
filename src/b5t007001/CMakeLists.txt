set (libname "b5t007001")
set (libdescription "B5T HVC Sensor")

add_subdirectory(stblib)

set (module_src "${libname}.cxx" "b5t007001.cxx")
set (module_hpp "${libname}.hpp" "b5t007001.hpp")

set (module_hpp ${libname}.hpp stblib/usr_include/STBAPI.h stblib/usr_include/STBCommonDef.h stblib/usr_include/STBTypedef.h)

include_directories("stblib/src/include/")
include_directories("stblib/usr_include")

upm_module_init(mraa)
upm_module_init(stblib)
