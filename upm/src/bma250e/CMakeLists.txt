upm_mixed_module_init (NAME bma250e
    DESCRIPTION "3-Axis Digital Accelerometer"
    C_HDR bma250e.h bma250e_defs.h
    C_SRC bma250e.c
    CPP_HDR bma250e.hpp
    CPP_SRC bma250e.cxx
    FTI_SRC bma250e_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
