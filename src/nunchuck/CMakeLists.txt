upm_mixed_module_init (NAME nunchuck
    DESCRIPTION "I2C nunchuck driver"
    C_HDR nunchuck.h
    C_SRC nunchuck.c
    CPP_HDR nunchuck.hpp
    CPP_SRC nunchuck.cxx
    FTI_SRC nunchuck_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
