# Create a source file for constants
configure_file (iUpmObject_generated.cxx.in iUpmObject_generated.cxx @ONLY)

upm_mixed_module_init (NAME core
    DESCRIPTION "CXX Interface Library"
    CPP_HDR iADC.hpp
            iCO2Sensor.hpp
            iHumiditySensor.hpp
            iLightController.hpp
            iLightSensor.hpp
            iMoistureSensor.hpp
            iMraa.hpp
            iPressureSensor.hpp
            iSensorType.hpp
            iActuatorType.hpp
            iServoActuator.hpp
            iTemperatureSensor.hpp
            iUpmObject.hpp
    CPP_SRC "${CMAKE_CURRENT_BINARY_DIR}/iUpmObject_generated.cxx"
            iADC.cxx
            iCO2Sensor.cxx
            iHumiditySensor.cxx
            iLightController.cxx
            iLightSensor.cxx
            iMoistureSensor.cxx
            iMraa.cxx
            iPressureSensor.cxx
            iSensorType.cxx
            iActuatorType.cxx
            iServoActuator.cxx
            iTemperatureSensor.cxx
            iUpmObject.cxx
    REQUIRES mraa utilities-c)

# Add a PUBLIC include directory to the CMAKE src dir
target_include_directories (core
    PUBLIC ${CMAKE_SOURCE_DIR}/src
           ${CMAKE_BINARY_DIR}/src)
