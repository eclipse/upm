upm_mixed_module_init (NAME hka5
    DESCRIPTION "UPM driver for DFRobot HKA5 PM2.5 particle sensor"
    C_HDR hka5.h
    C_SRC hka5.c
    CPP_HDR hka5.hpp
    CPP_SRC hka5.cxx
    FTI_SRC hka5_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
