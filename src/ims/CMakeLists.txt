upm_mixed_module_init (NAME ims
    DESCRIPTION "I2C Moisture Sensor"
    C_HDR ims.h ims_defs.h
    C_SRC ims.c
    CPP_HDR ims.hpp
    CPP_SRC ims.cxx
    FTI_SRC ims_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
