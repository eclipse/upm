upm_mixed_module_init (NAME lcm1602
    DESCRIPTION "LCD display driver for the LCM1602 controller for HD44780-based displays"
    C_HDR lcm1602.h
    C_SRC lcm1602.c
    CPP_HDR lcm1602.hpp
    CPP_SRC lcm1602.cxx
#    FTI_SRC lcm1602_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
