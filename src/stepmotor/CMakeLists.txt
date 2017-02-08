# Currently no librt in android
if (NOT ANDROID)
    set (libname "stepmotor")
    set (libdescription "Stepper Motor")
    set (module_src ${libname}.cxx)
    set (module_hpp ${libname}.hpp)
    upm_module_init(mraa)
    target_link_libraries(${libname} rt)
endif (NOT ANDROID)
