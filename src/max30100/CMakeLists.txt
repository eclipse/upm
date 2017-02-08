upm_mixed_module_init (NAME max30100
    DESCRIPTION "Pulse Oximeter and Heart-rate Sensor"
    C_HDR max30100.h
    C_SRC max30100.c
    CPP_HDR max30100.hpp
    CPP_SRC max30100.cxx
    FTI_SRC max30100_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
