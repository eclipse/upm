upm_mixed_module_init (NAME bmp280
    DESCRIPTION "Bosch Atmospheric Sensor Library (BMP280 and BME280)"
    C_HDR bmp280.h bmp280_regs.h
    C_SRC bmp280.c
    CPP_HDR bmp280.hpp bme280.hpp
    CPP_SRC bmp280.cxx bme280.cxx
    CPP_WRAPS_C
    REQUIRES mraa interfaces utilities-c)
target_link_libraries(${libnamec} m)
