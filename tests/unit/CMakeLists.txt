# For now, Google Test is NOT required */
find_package(GTest)

# If not found, print a status message and return
if(NOT GTEST_FOUND)
    message(STATUS "Install Google Test to enable additional unit testing")
    return ()
endif()

# Unit tests - utilities library
add_executable(utilities_tests utilities/utilities_tests.cxx)
target_link_libraries(utilities_tests utilities GTest::GTest GTest::Main)
gtest_add_tests(utilities_tests "" AUTO)

# Unit tests - Json header
add_executable(json_tests json/json_tests.cxx)
target_link_libraries(json_tests GTest::GTest GTest::Main)
target_include_directories(json_tests PRIVATE "${UPM_COMMON_HEADER_DIRS}/")
gtest_add_tests(json_tests "" AUTO)

# Unit tests - logger header library
add_executable(logger_tests logger/logger_tests.cxx)
target_link_libraries(logger_tests pthread GTest::GTest GTest::Main)
target_include_directories(logger_tests PRIVATE "${UPM_COMMON_HEADER_DIRS}/")
gtest_add_tests(logger_tests "" AUTO)

# Unit tests - nmea_gps library
add_executable(nmea_gps_tests nmea_gps/nmea_gps_tests.cxx)
target_link_libraries(nmea_gps_tests nmea_gps GTest::GTest GTest::Main)
gtest_add_tests(nmea_gps_tests "" AUTO)

# Add a custom target for unit tests
add_custom_target(tests-unit ALL
    DEPENDS
    utilities_tests
    json_tests
    logger_tests
    COMMENT "UPM unit test collection")

