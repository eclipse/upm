upm_mixed_module_init (NAME loudness
    DESCRIPTION "upm loudness sensor module"
    C_HDR loudness.h
    C_SRC loudness.c
    CPP_SRC groveloudness.cxx
    CPP_WRAPS_C
    REQUIRES libmraa)
