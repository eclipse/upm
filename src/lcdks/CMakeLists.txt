upm_mixed_module_init (NAME lcdks
    DESCRIPTION "LCD Display Driver for LCD Keypad Shield devices based on the LCM1602"
    C_HDR lcdks.h
    C_SRC lcdks.c
    CPP_HDR lcdks.hpp
    CPP_SRC lcdks.cxx
    CPP_WRAPS_C
    REQUIRES mraa lcm1602)
