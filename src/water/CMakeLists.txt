upm_mixed_module_init (NAME water
    DESCRIPTION "Water detection sensor"
    C_HDR water.h
    C_SRC water.c
    CPP_HDR water.hpp
    CPP_SRC water.cxx
    FTI_SRC water_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
