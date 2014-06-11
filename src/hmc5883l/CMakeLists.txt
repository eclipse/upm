set (libname "hmc5883l")
set (libdescription "libupm Digital Compass")
add_library (hmc5883l SHARED hmc5883l.cxx)
upm_module_init()
