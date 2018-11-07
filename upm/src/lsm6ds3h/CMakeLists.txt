upm_mixed_module_init (NAME lsm6ds3h
    DESCRIPTION "3-Axis Digital Accelerometer and Gyroscope"
    C_HDR lsm6ds3h.h lsm6ds3h_defs.h
    C_SRC lsm6ds3h.c
    CPP_HDR lsm6ds3h.hpp
    CPP_SRC lsm6ds3h.cxx
    FTI_SRC lsm6ds3h_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
