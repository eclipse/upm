upm_mixed_module_init (NAME loudness
    DESCRIPTION "Loudness sensor"
    C_HDR loudness.h
    C_SRC loudness.c
    CPP_SRC loudness.cxx
    CPP_HDR loudness.hpp
    FTI_SRC loudness_fti.c
    CPP_WRAPS_C
    REQUIRES libmraa)
