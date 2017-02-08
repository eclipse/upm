upm_mixed_module_init (NAME ppd42ns
    DESCRIPTION "Dust Sensor"
    C_HDR ppd42ns.h ppd42ns_data.h
    C_SRC ppd42ns.c
    CPP_HDR ppd42ns.hpp
    CPP_SRC ppd42ns.cxx
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
target_link_libraries(${libnamec} m)
