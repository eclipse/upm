upm_mixed_module_init (NAME mma7361
    DESCRIPTION "Triaxial analog accelerometer"
    C_HDR mma7361.h
    C_SRC mma7361.c
    CPP_HDR mma7361.hpp
    CPP_SRC mma7361.cxx
    FTI_SRC mma7361_fti.c
    CPP_WRAPS_C
    REQUIRES upmc-utilities mraa)
