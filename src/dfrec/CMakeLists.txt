if (MRAA_OW_FOUND)
    upm_mixed_module_init (NAME dfrec
        DESCRIPTION "Analog Electrical Conductivity (EC) Sensor"
        C_HDR dfrec.h
        C_SRC dfrec.c
        CPP_HDR dfrec.hpp
        CPP_SRC dfrec.cxx
        FTI_SRC dfrec_fti.c
        CPP_WRAPS_C
        REQUIRES ds18b20 mraa utilities-c)
    # make sure the C library has the appropriate dependency on the UPM
    # DS18B20 C library
    target_link_libraries(${libnamec} ds18b20-c)
endif (MRAA_OW_FOUND)
