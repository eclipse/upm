# we need includes from ds18b20
include_directories("../ds18b20")

upm_mixed_module_init (NAME dfrec
    DESCRIPTION "DFRobot analog electrical conductivity (EC) sensor"
    C_HDR dfrec.h
    C_SRC dfrec.c
    CPP_HDR dfrec.hpp
    CPP_SRC dfrec.cxx
    FTI_SRC dfrec_fti.c
    CPP_WRAPS_C
    REQUIRES upmc-ds18b20 mraa)

# make sure the C library has the appropriate dependency on the UPM
# DS18B20 C library
add_dependencies(${libnamec} ds18b20-c)
target_link_libraries(${libnamec} ds18b20-c)
