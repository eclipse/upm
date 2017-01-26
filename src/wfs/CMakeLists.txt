upm_mixed_module_init (NAME wfs
    DESCRIPTION "WFS (Water Flow Sensor)"
    C_HDR wfs.h
    C_SRC wfs.c
    CPP_HDR wfs.hpp
    CPP_SRC wfs.cxx
    CPP_WRAPS_C
    REQUIRES mraa)
