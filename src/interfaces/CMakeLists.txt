set (libname "interfaces")
set (libdescription "CXX Interface Library")
set (module_src ${libname}.cxx)

upm_module_init()

# Add a PUBLIC include directory to the CMAKE src dir
target_include_directories (${libname} PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Don't add the hpp files with upm_module_init, this allows
# them to be installed separately
set (module_hpp iADC.hpp
                iCO2Sensor.hpp
                iHumiditySensor.hpp
                iLightController.hpp
                iLightSensor.hpp
                iModuleStatus.hpp
                iPressureSensor.hpp
                iTemperatureSensor.hpp)
# Install interfaces headers a bit differently
install (FILES ${module_hpp} DESTINATION include/upm/${libname}
    COMPONENT ${CMAKE_PROJECT_NAME})
