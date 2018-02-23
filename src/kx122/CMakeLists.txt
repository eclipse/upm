upm_mixed_module_init (NAME kx122
    DESCRIPTION "3-Axis Digital Accelerometer"
    C_HDR kx122.h kx122_registers.h
    C_SRC kx122.c
    CPP_HDR kx122.hpp
    CPP_SRC kx122.cxx
    CPP_WRAPS_C
    REQUIRES mraa m)
