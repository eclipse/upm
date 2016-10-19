upm_mixed_module_init (NAME bh1750
    DESCRIPTION "Digital light sensor"
    C_HDR bh1750.h
    C_SRC bh1750.c
    CPP_HDR bh1750.hpp
    CPP_SRC bh1750.cxx
    FTI_SRC bh1750_fti.c
    CPP_WRAPS_C
    REQUIRES upmc-utilities mraa)
