if (MRAA_OW_FOUND)
    upm_mixed_module_init (NAME ds18b20
        DESCRIPTION "Programmable Resolution 1-Wire Digital Thermometer"
        C_HDR ds18b20.h ds18b20_defs.h
        C_SRC ds18b20.c
        CPP_HDR ds18b20.hpp
        CPP_SRC ds18b20.cxx
        FTI_SRC ds18b20_fti.c
        CPP_WRAPS_C
        REQUIRES mraa utilities-c)
endif (MRAA_OW_FOUND)
