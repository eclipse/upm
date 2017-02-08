upm_mixed_module_init (NAME bmi160
    DESCRIPTION "Bosch Sensortec IMU (triaxial Accelerometer, Triaxial Gyroscope and Magnetometer Interface)"
    C_HDR bmi160.h bmi160_defs.h bosch_bmi160.h
    C_SRC bmi160.c bosch_bmi160.c
    CPP_HDR bmi160.hpp
    CPP_SRC bmi160.cxx
#    FTI_SRC bmi160_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
