set (libname "tb7300")
set (libdescription "Module for the TB7300 Fan Coil Thermostat")
set (module_src ${libname}.cxx)
set (module_hpp ${libname}.hpp)

if (BACNET_FOUND)
  # upm-libbacnetmstp will bring in libbacnet, I hope
  set (reqlibname "upm-bacnetmstp")
  include_directories(${BACNET_INCLUDE_DIRS})
  include_directories("../bacnetmstp")
  upm_module_init()
  upm_target_link_libraries(${libname} bacnetmstp)
endif ()
