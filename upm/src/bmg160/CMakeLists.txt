upm_mixed_module_init (NAME bmg160
    DESCRIPTION "3-Axis Digital Gyroscope"
    C_HDR bmg160.h bmg160_defs.h
    C_SRC bmg160.c
    CPP_HDR bmg160.hpp
    CPP_SRC bmg160.cxx
    FTI_SRC bmg160_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
