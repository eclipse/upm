upm_mixed_module_init (NAME jhd1313m1
    DESCRIPTION "LCD display driver for the JHD1313M1 controller for HD44780-based displays"
    C_HDR jhd1313m1.h
    C_SRC jhd1313m1.c
    CPP_HDR jhd1313m1.hpp
    CPP_SRC jhd1313m1.cxx
#    FTI_SRC jhd1313m1_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
# We need hd44780_bits.h from lcm1602
include_directories("../lcm1602")
