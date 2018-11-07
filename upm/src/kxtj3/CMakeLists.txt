upm_mixed_module_init (NAME kxtj3
    DESCRIPTION "Tri-Axis Accelerometer"
    C_HDR kxtj3.h kxtj3_registers.h
    C_SRC kxtj3.c
    CPP_HDR kxtj3.hpp
    CPP_SRC kxtj3.cxx
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
