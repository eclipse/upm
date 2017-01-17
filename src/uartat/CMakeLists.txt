upm_mixed_module_init (NAME uartat
    DESCRIPTION "Generic UART driver for serial AT command driven devices"
    C_HDR uartat.h uartat_defs.h
    C_SRC uartat.c
    CPP_HDR uartat.hpp
    CPP_SRC uartat.cxx
    CPP_WRAPS_C
    REQUIRES mraa)
