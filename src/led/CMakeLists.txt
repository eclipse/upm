upm_mixed_module_init (NAME led
    DESCRIPTION "LED module"
    C_HDR led.h
    C_SRC led.c
    CPP_HDR led.hpp
    CPP_SRC led.cxx
    FTI_SRC led_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
