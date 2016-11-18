upm_mixed_module_init (NAME ims
    DESCRIPTION "Catnip Electronics I2C moisture sensor"
    C_HDR ims.h
    C_SRC ims.c
    CPP_HDR ims.hpp
    CPP_SRC ims.cxx
    FTI_SRC ims_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
