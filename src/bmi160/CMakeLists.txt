upm_mixed_module_init (NAME bmi160
    DESCRIPTION "Bosch Sensortec IMU (triaxial accelerometer, triaxial gyroscope and magnetometer interface)"
    C_HDR bmi160.h bosch_bmi160.h
    C_SRC bmi160.c bosch_bmi160.c
    CPP_HDR bmi160.hpp
    CPP_SRC bmi160.cxx
#    FTI_SRC bmi160_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
