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

# Unit tests - core library
add_executable(core_tests
    core/iUpmObject_tests.cxx

    core/iSensortType_tests.cxx

    core/iActuatorType_tests.cxx
    core/iServoActuator_tests.cxx

    core/sensortype_example.json
    core/actuatortype_example.json)

configure_file(core/sensortype_example.json sensortype_example.json COPYONLY)
configure_file(core/actuatortype_example.json actuatortype_example.json COPYONLY)

target_link_libraries(core_tests core GTest::GTest GTest::Main)
gtest_add_tests(core_tests "" AUTO)

# Add a custom target for unit tests
add_custom_target(tests-unit ALL
    DEPENDS
    utilities_tests
    json_tests
    core_tests
    COMMENT "UPM unit test collection")

