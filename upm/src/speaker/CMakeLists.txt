upm_mixed_module_init (NAME speaker
    DESCRIPTION "Speaker"
    C_HDR speaker.h
    C_SRC speaker.c
    CPP_HDR speaker.hpp
    CPP_SRC speaker.cxx
#    FTI_SRC speaker_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
