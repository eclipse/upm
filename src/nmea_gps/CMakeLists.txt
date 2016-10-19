upm_mixed_module_init (NAME nmea_gps
    DESCRIPTION "Generic driver for GPS NMEA sensors"
    C_HDR nmea_gps.h
    C_SRC nmea_gps.c
    CPP_HDR nmea_gps.hpp
    CPP_SRC nmea_gps.cxx
    FTI_SRC nmea_gps_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
