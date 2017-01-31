if (BACNET_FOUND)
  set (libname "e50hx")
  set (libdescription "Module for the Veris E50HX (E50H2/E50H5)Energy Meters")
  set (module_src ${libname}.cxx)
  set (module_hpp ${libname}.hpp)

  upm_module_init(bacnetmstp)
endif ()
