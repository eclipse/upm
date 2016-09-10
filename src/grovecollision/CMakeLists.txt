upm_mixed_module_init (NAME grovecollision
    DESCRIPTION "upm grovecollisionsensor module"
    C_HDR grovecollision.h
    C_SRC grovecollision.c
    CPP_HDR grovecollision.hpp
    CPP_SRC grovecollision.cxx
    FTI_SRC grovecollision_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)