set (libname "my9221")
set (libdescription "upm ledbar")
add_library (${libname} SHARED my9221.cxx)
upm_module_init()
