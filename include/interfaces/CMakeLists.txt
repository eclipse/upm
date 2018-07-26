set (libname "new_interfaces")
set (libdescription "CXX Interface Library")
set (module_src ${libname}.cxx)

upm_module_init()

# Add a PUBLIC include directory to the CMAKE src dir
target_include_directories (${libname} PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Don't add the hpp files with upm_module_init, this allows
# them to be installed separately
set (module_hpp iClock.hpp
                iCollision.hpp
                iDistance.hpp
                iDistanceInterrupter.hpp
                iEC.hpp
                iEmg.hpp
                iHallEffect.hpp
                iHeartRate.hpp
                iHumidity.hpp
                iLight.hpp
                iLineFinder.hpp
                iMoisture.hpp
                iMotion.hpp
                iPressure.hpp
                iTemperature.hpp
                iAcceleration.hpp
)
# Install interfaces headers a bit differently
install (FILES ${module_hpp} DESTINATION include/upm/interfaces COMPONENT ${libname})
