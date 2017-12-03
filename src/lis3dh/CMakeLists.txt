upm_mixed_module_init (NAME lis3dh
    DESCRIPTION "3-Axis Digital Accelerometer"
    C_HDR lis3dh.h lis3dh_defs.h
    C_SRC lis3dh.c
    CPP_HDR lis3dh.hpp
    CPP_SRC lis3dh.cxx
    FTI_SRC lis3dh_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
