upm_mixed_module_init (NAME lsm6dsl
    DESCRIPTION "3-Axis Digital Accelerometer and Gyroscope"
    C_HDR lsm6dsl.h lsm6dsl_defs.h
    C_SRC lsm6dsl.c
    CPP_HDR lsm6dsl.hpp
    CPP_SRC lsm6dsl.cxx
    FTI_SRC lsm6dsl_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
