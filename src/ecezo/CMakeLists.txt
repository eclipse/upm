upm_mixed_module_init (NAME ecezo
    DESCRIPTION "EC EZO Atlas Scientific EC circuit"
    C_HDR ecezo.h ecezo_defs.h
    C_SRC ecezo.c
    CPP_HDR ecezo.hpp
    CPP_SRC ecezo.cxx
    FTI_SRC ecezo_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
