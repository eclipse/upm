if (BACNET_FOUND)
    set (libname "t8100")
    set (libdescription "Module for the Telaire T8100 Ventostat")
    set (module_src ${libname}.cxx)
    set (module_hpp ${libname}.hpp)

    upm_module_init(bacnetmstp)
endif ()
