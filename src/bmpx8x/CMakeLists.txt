upm_mixed_module_init (NAME bmpx8x
    DESCRIPTION "Pressure and temperature sensor"
    C_HDR bmpx8x.h bmpx8x_defs.h
    C_SRC bmpx8x.c
    CPP_HDR bmpx8x.hpp
    CPP_SRC bmpx8x.cxx
    FTI_SRC bmpx8x_fti.c
    CPP_WRAPS_C
    REQUIRES mraa interfaces utilities-c)
target_link_libraries(${libnamec} m)
