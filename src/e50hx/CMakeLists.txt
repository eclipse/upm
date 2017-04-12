if (BACNET_FOUND)
  set (libname "e50hx")
  set (libdescription "Veris E50HX (E50H2/E50H5) Energy Meter Module")
  set (module_src ${libname}.cxx)
  set (module_hpp ${libname}.hpp)

  upm_module_init(bacnetmstp)
endif ()
