upm_mixed_module_init (NAME bmx055
    DESCRIPTION "Bosch IMU Sensor Library"
    CPP_HDR bmx055.hpp bmc150.hpp bmi055.hpp
    CPP_SRC bmx055.cxx bmc150.cxx bmi055.cxx
    IFACE_HDR iAcceleration.hpp iGyroscope.hpp
    REQUIRES mraa bmg160 bma250e bmm150)
