set (libname "grove")
set (libdescription "upm grove module")
add_library (grove SHARED grove.cxx)
upm_module_init()
