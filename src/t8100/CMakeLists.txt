if (BACNET_FOUND)
    set (libname "t8100")
    set (libdescription "Wall Mount CO2, Humidity and Temperature Transmitter")
    set (module_src ${libname}.cxx)
    set (module_hpp ${libname}.hpp)

    upm_module_init(bacnetmstp)
endif ()
