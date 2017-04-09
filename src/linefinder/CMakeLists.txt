upm_mixed_module_init (NAME linefinder
    DESCRIPTION "Infrared (IR) Based Line Finder"
    C_HDR linefinder.h
    C_SRC linefinder.c
    CPP_HDR linefinder.hpp
    CPP_SRC linefinder.cxx
    FTI_SRC linefinder_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
