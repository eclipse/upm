upm_mixed_module_init (NAME ms5803
    DESCRIPTION "MS5803 Pressure and temperature sensor"
    C_HDR ms5803.h ms5803_defs.h
    C_SRC ms5803.c
    CPP_HDR ms5803.hpp
    CPP_SRC ms5803.cxx
    FTI_SRC ms5803_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
