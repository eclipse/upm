upm_mixed_module_init (NAME temperature
    DESCRIPTION "Analog temperature sensor"
    C_HDR temperature.h
    C_SRC temperature.c
    CPP_HDR temperature.hpp
    CPP_SRC temperature.cxx
    FTI_SRC temperature_fti.c
    REQUIRES mraa)
