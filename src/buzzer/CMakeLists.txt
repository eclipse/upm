set (libname "buzzer")
set (libdescription "upm buzzer")
add_library (buzzer SHARED buzzer.cxx)
upm_module_init()
