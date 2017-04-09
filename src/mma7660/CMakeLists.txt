upm_mixed_module_init (NAME mma7660
    DESCRIPTION "I2C 3-axis Digital Accelerometer (1.5g)"
    C_HDR mma7660.h mma7660_regs.h
    C_SRC mma7660.c
    CPP_HDR mma7660.hpp
    CPP_SRC mma7660.cxx
    FTI_SRC mma7660_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
