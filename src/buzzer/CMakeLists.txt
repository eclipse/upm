upm_mixed_module_init (NAME buzzer
    DESCRIPTION "Buzzer"
    C_HDR buzzer.h buzzer_tones.h
    C_SRC buzzer.c
    CPP_HDR buzzer.hpp
    CPP_SRC buzzer.cxx
#    FTI_SRC buzzer_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
