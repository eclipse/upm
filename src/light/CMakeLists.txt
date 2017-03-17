upm_mixed_module_init (NAME light
    DESCRIPTION "Analog Light Sensor"
    C_HDR light.h
    C_SRC light.c
    CPP_HDR light.hpp
    CPP_SRC light.cxx
    FTI_SRC light_fti.c
    CPP_WRAPS_C
    REQUIRES mraa m)
target_link_libraries(light-c m)
