set (libname "hcsr04")
set (libdescription "upm proximity sensor")
add_library (hcsr04 SHARED hcsr04.cxx)
upm_module_init()
