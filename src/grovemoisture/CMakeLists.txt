upm_mixed_module_init (NAME grovemoisture
    DESCRIPTION "upm grove moisture module"
    C_HDR moisture.h
    C_SRC moisture.c
    CPP_HDR grovemoisture.hpp
    CPP_SRC grovemoisture.cxx
    FTI_SRC grovemoisture_fti.c
    CPP_WRAPS_C
    REQUIRES libmraa)
