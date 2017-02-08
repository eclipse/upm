upm_mixed_module_init (NAME nunchuck
    DESCRIPTION "Wii Nunchuk"
    C_HDR nunchuck.h
    C_SRC nunchuck.c
    CPP_HDR nunchuck.hpp
    CPP_SRC nunchuck.cxx
    FTI_SRC nunchuck_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
