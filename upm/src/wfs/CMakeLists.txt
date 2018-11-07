upm_mixed_module_init (NAME wfs
    C_HDR wfs.h
    C_SRC wfs.c
    DESCRIPTION "WFS (Water Flow Sensor)"
    CPP_HDR wfs.hpp
    CPP_SRC wfs.cxx
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
