upm_mixed_module_init (NAME rotaryencoder
    DESCRIPTION "Rotary Encoder"
    C_HDR rotaryencoder.h
    C_SRC rotaryencoder.c
    CPP_HDR rotaryencoder.hpp
    CPP_SRC rotaryencoder.cxx
    FTI_SRC rotaryencoder_fti.c
    IFACE_HDR iAngle.hpp
    CPP_WRAPS_C
    REQUIRES mraa)
