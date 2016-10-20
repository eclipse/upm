set (libname "e50hx")
set (libdescription "Module for the Veris E50HX (E50H2/E50H5)Energy Meters")
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
