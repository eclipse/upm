upm_mixed_module_init (NAME md
    DESCRIPTION "I2C Motor Driver"
    C_HDR md.h md_defs.h
    C_SRC md.c
    CPP_HDR md.hpp
    CPP_SRC md.cxx
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
