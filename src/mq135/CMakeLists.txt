set (libname "mq135")
set (libdescription "upm mq135 module")
set (module_src ${libname}.cxx)
set (module_h ${libname}.h)
upm_module_init()
target_link_libraries(${libname} m)

