upm_mixed_module_init (NAME lis2ds12
    DESCRIPTION "3-Axis Digital Accelerometer"
    C_HDR lis2ds12.h lis2ds12_defs.h
    C_SRC lis2ds12.c
    CPP_HDR lis2ds12.hpp
    CPP_SRC lis2ds12.cxx
    FTI_SRC lis2ds12_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
