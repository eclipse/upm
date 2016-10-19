upm_mixed_module_init (NAME servo
    DESCRIPTION "Servo controller"
    C_HDR es08a.h es9257.h
    C_SRC es08a.c es9257.c
    CPP_HDR servo.hpp es08a.hpp es9257.hpp
    CPP_SRC servo.cxx es08a.cxx es9257.cxx
    FTI_SRC es08a_fti.c es9257_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
