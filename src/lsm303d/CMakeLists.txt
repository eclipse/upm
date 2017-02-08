upm_mixed_module_init (NAME lsm303d
    DESCRIPTION "3-Axis eCompass Module"
    C_HDR lsm303d.h lsm303d_defs.h
    C_SRC lsm303d.c
    CPP_HDR lsm303d.hpp
    CPP_SRC lsm303d.cxx
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
