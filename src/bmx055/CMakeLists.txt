upm_mixed_module_init (NAME bmx055
    DESCRIPTION "Bosch IMU Sensor Library"
    CPP_HDR bmx055.hpp bmm150.hpp bmc150.cxx bmi055.hpp
    CPP_SRC bmx055.cxx bmm150.cxx bmc150.cxx bmi055.cxx
    REQUIRES mraa bmg160 bma250e)
