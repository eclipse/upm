upm_mixed_module_init (NAME sht1x
    DESCRIPTION "Temperature and Humidity Sensor"
    C_HDR sht1x.h sht1x_defs.h
    C_SRC sht1x.c
    CPP_HDR sht1x.hpp
    CPP_SRC sht1x.cxx
    FTI_SRC sht1x_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
