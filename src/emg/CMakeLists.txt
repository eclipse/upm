upm_mixed_module_init (NAME emg
    DESCRIPTION "Electromyography (EMG) Sensor"
    C_HDR emg.h
    C_SRC emg.c
    CPP_HDR emg.hpp
    CPP_SRC emg.cxx
    FTI_SRC emg_fti.c
    REQUIRES mraa)
