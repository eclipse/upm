upm_mixed_module_init (NAME dfrorp
    DESCRIPTION "Analog Oxidation Reduction Potential (ORP) Sensor"
    C_HDR dfrorp.h
    C_SRC dfrorp.c
    CPP_HDR dfrorp.hpp
    CPP_SRC dfrorp.cxx
    FTI_SRC dfrorp_fti.c
    IFACE_HDR iOrp.hpp
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
