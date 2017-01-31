if (BACNET_FOUND)
  set (libname "tb7300")
  set (libdescription "Module for the TB7300 Fan Coil Thermostat")
  set (module_src ${libname}.cxx)
  set (module_hpp ${libname}.hpp)

  upm_module_init(bacnetmstp)
endif ()
