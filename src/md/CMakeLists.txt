upm_mixed_module_init (NAME md
    DESCRIPTION "I2C motor driver"
    C_HDR md.h
    C_SRC md.c
    CPP_HDR md.hpp
    CPP_SRC md.cxx
#    FTI_SRC md_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
