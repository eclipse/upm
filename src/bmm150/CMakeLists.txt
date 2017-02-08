upm_mixed_module_init (NAME bmm150
    DESCRIPTION "3-Axis Digital Magnetometer"
    C_HDR bmm150.h bmm150_defs.h
    C_SRC bmm150.c
    CPP_HDR bmm150.hpp
    CPP_SRC bmm150.cxx
    FTI_SRC bmm150_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
