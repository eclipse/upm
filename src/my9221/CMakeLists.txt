set (libname "my9221")
set (libdecription "upm ledbar")
add_library (${libname} SHARED my9221.cxx)
upm_module_init()
