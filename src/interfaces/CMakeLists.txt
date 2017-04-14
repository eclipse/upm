set (libname "interfaces")
set (libdescription "CXX Interface Library")
set (module_src ${libname}.cxx)

upm_module_init()

# Add a PUBLIC include directory to the CMAKE src dir
target_include_directories (${libname} PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Don't add the hpp files with upm_module_init, this allows
# them to be installed separately
set (module_hpp iADC.hpp
                iAngle.hpp
                iAudio.hpp
                iBinary.hpp
                iCurrent.hpp
                iDistance.hp
                iEC.hpp
                iFlow.hpp
                iForce.hpp
                iGas.hpp
                iGyroscope.hpp
                iHeartRate.hpp
                iHumidity.hpp
                iJoystick.hpp
                iLightController.hpp
                iLight.hpp
                iMagnetometer.hpp
                iMoisture.hpp
                ipH.hpp
                iPotentiometer.hpp
                iPressure.hpp
                iServo.hpp
                iStepper.hpp
                iSwitch.hpp
                iTemperature.hpp
                iThermostat.hpp
                iTime.hpp
                iVoltage.hpp
                iUPMActuator.hpp
                iUPMProtocol.hpp
                iUPMRadio.hpp
                iUPMSensor.hpp
                iUPMObject.hpp)
# Install interfaces headers a bit differently
install (FILES ${module_hpp} DESTINATION include/upm/${libname} COMPONENT ${libname})
