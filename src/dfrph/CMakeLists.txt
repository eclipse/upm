upm_mixed_module_init (NAME dfrph
    DESCRIPTION "Analog pH Sensor"
    C_HDR dfrph.h
    C_SRC dfrph.c
    CPP_HDR dfrph.hpp
    CPP_SRC dfrph.cxx
    FTI_SRC dfrph_fti.c
    IFACE_HDR iPH.hpp
    CPP_WRAPS_C
    REQUIRES mraa)
