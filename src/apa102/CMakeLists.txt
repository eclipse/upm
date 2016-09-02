upm_mixed_module_init (NAME apa102
    DESCRIPTION "UPM driver for DFRobot HKA5 PM2.5 particle sensor"
    C_HDR apa102.h
    C_SRC apa102.c
    CPP_HDR apa102.hpp
    CPP_SRC apa102.cxx
    FTI_SRC apa102_fti.c
    REQUIRES upmc-utilities mraa)