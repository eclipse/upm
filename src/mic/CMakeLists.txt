upm_mixed_module_init (NAME mic
    DESCRIPTION "Simple analog microphone"
    C_HDR mic.h
    C_SRC mic.c
    CPP_HDR mic.hpp
    CPP_SRC mic.cxx
    FTI_SRC mic_fti.c
    REQUIRES mraa)
