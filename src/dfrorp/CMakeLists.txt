upm_mixed_module_init (NAME dfrorp
    DESCRIPTION "DFRobot analog oxidation reduction potential (ORP) sensor"
    C_HDR dfrorp.h
    C_SRC dfrorp.c
    CPP_HDR dfrorp.hpp
    CPP_SRC dfrorp.cxx
    FTI_SRC dfrorp_fti.c
    CPP_WRAPS_C
    REQUIRES upmc-utilities mraa)
