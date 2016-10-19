upm_mixed_module_init (NAME moisture
    DESCRIPTION "Moisture sensor"
    C_HDR moisture.h
    C_SRC moisture.c
    CPP_HDR moisture.hpp
    CPP_SRC moisture.cxx
    FTI_SRC moisture_fti.c
    CPP_WRAPS_C
    REQUIRES libmraa)
