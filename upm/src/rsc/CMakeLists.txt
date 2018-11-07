upm_mixed_module_init (NAME rsc
    DESCRIPTION "Honeywell RSC TruStability Pressure and Temperature Sensor"
    C_HDR rsc.h rsc_regs.h
    C_SRC rsc.c
    CPP_HDR rsc.hpp
    CPP_SRC rsc.cxx
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
