set (libname "tm1637")
set (libdescription "upm 4 digit display tm1637")
add_library (${libname} SHARED tm1637.cxx)
upm_module_init()
